#include "tripch.h"

#include "ScriptEngine.h"

#include "Utils/PlatformUtils.h"

#include "ScriptBinding.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Components.h"

#include "Core/Projects/ProjectManager.h"

#include "scriptbuilder/scriptbuilder.h"
#include "asbind20/asbind.hpp"

static void MessageCallback(const asSMessageInfo* msg, void* param)
{
	const char* type = "ERROR";
	if( msg->type == asMSGTYPE_WARNING ) {
        type = "WARN";
        TRI_CORE_WARN("{0} ({1}, {2}) : {3} : {4}", msg->section, msg->row, msg->col, type, msg->message);
    }
    else if( msg->type == asMSGTYPE_INFORMATION ) {
        type = "INFO";
        TRI_CORE_INFO("{0} ({1}, {2}) : {3} : {4}", msg->section, msg->row, msg->col, type, msg->message);
    }
    else if( msg->type == asMSGTYPE_ERROR ) {
        type = "ERROR";
        TRI_CORE_ERROR("{0} ({1}, {2}) : {3} : {4}", msg->section, msg->row, msg->col, type, msg->message);
    }
}

namespace TriEngine {

    int ByteCodeStream::Write(const void* ptr, uint32_t size) {
        if (size == 0)
            return -1;

        if (m_Buffer.size() < m_CurrentPos + size)
            m_Buffer.resize(m_CurrentPos + size);

        memcpy(&m_Buffer[m_CurrentPos], ptr, size);
        m_CurrentPos += size;
        return 0;
    }

    int ByteCodeStream::Read(void* ptr, uint32_t size) {
        // The buffer will be empty if there was an error compiling the script
        if (size == 0 || m_Buffer.empty())
            return -1;
        TRI_CORE_ASSERT(m_Buffer.size() >= m_CurrentPos + size, "Attempted to read past the size of the buffer");
        memcpy(ptr, &m_Buffer[m_CurrentPos], size);
        m_CurrentPos += size;
        return 0;
    }

    ScriptEngine& ScriptEngine::Get() {
        static ScriptEngine instance;
        return instance;
    }

    ScriptEngine::ScriptEngine()
        :m_Engine(nullptr)
    {
        m_Engine = asCreateScriptEngine();
        TRI_CORE_ASSERT(m_Engine != nullptr, "Failed to create script engine");

        // Performance increase
        m_Engine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);

        // TODO: do this in a better way
        if (ProjectManager::GetCurrentProjectData().Binary)
            // Needed for bytecode loading to work for some reason
            m_Engine->SetEngineProperty(asEP_INIT_GLOBAL_VARS_AFTER_BUILD, false);

        m_Engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
        Utils::ConfigureScriptEngine(m_Engine);

        m_Context = m_Engine->CreateContext();
        TRI_CORE_ASSERT(m_Context != 0, "Failed to create script context");
    }

    ScriptEngine::~ScriptEngine() 
    {
        m_Context->Release();
        m_Engine->ShutDownAndRelease();
    }

    void ScriptEngine::BuildScript(Reference<Script> script) {
        TRI_CORE_ASSERT(script, "Invalid script resource");

        if (!std::filesystem::exists(script->MetaData.Filepath) && script->Bytecode.empty()) [[unlikely]]
        {
            TRI_CORE_ERROR("Couldn't build script '{0}': invalid file path or no valid bytecode", script->MetaData.Filepath);
            return;
        }

        auto it = std::find_if(
            m_Scripts.begin(),
            m_Scripts.end(),
            [&](std::weak_ptr<Script> const &w)
            {
                auto ref = w.lock();
                return ref && ref == script;
            });

        bool alreadyExists = it != m_Scripts.end();

        if (!alreadyExists)
            m_Scripts.emplace_back(script);
        else {
            TRI_CORE_INFO("Rebuilding script '{}'", script->Name);
            script->Module->Discard();
            script->Module = nullptr;
            script->TypeInfo = nullptr;
        }

        std::string scriptName = std::filesystem::path(script->MetaData.Filepath).filename().generic_string();

        CScriptBuilder builder;
        int32_t r = builder.StartNewModule(m_Engine, script->Name.data());
        TRI_CORE_ASSERT(r >= 0, "Unrecoverable error while starting a new script module");

        asIScriptModule* module = m_Engine->GetModule(script->Name.data());

        if (!script->Bytecode.empty()) {
            ByteCodeStream stream(script->Bytecode);
            module->LoadByteCode(&stream, &script->HasDebugInfo);
        }
        else {
            r = builder.AddSectionFromFile(script->MetaData.Filepath.c_str());
            if (r < 0)
            {
                TRI_CORE_ERROR("Couldn't build script {0}: see build errors", script->MetaData.Filepath);
                return;
            }

            r = builder.BuildModule();
            if (r < 0)
            {
                TRI_CORE_ERROR("Couldn't build script {0}: see build errors", script->MetaData.Filepath);
                return;
            }
        }

        asITypeInfo* scriptTypeInfo = nullptr;

        int32_t objectTypeCount = module->GetObjectTypeCount();
        for (int32_t i = 0; i < objectTypeCount; i++) {
            asITypeInfo* type = module->GetObjectTypeByIndex(i);

            int32_t interfaceCount = type->GetInterfaceCount();
            if (interfaceCount) {
                if (interfaceCount > 1) 
                    TRI_CORE_WARN("Scripts should only inherit from ScriptInterface");
                asITypeInfo* baseType = type->GetInterface(0);
                if (strcmp("ScriptInterface", baseType->GetName()) == 0) {
                    if (scriptTypeInfo) {
                        TRI_CORE_WARN("'{}': Please only extend ScriptInterface once per file", scriptName);
                        continue;
                    }
                    scriptTypeInfo = type;
                }
            }
        }

        if (!scriptTypeInfo) {
            TRI_CORE_WARN("No class found that extends ScriptInterface in '{}'. Was there an error building the script?", scriptName);
            return;
        }

        script->Module = module;
        script->TypeInfo = scriptTypeInfo;

        script->StartFunc = scriptTypeInfo->GetMethodByDecl("void OnStart()");
        script->StopFunc = scriptTypeInfo->GetMethodByDecl("void OnStop()");
        script->UpdateFunc = scriptTypeInfo->GetMethodByDecl("void OnUpdate(float deltatime)");
        script->CollisionStartFunc = scriptTypeInfo->GetMethodByDecl("void OnCollisionStart(GameObject)");
        script->CollisionStopFunc = scriptTypeInfo->GetMethodByDecl("void OnCollisionStop(GameObject)");
    }

    void ScriptEngine::RebuildAllScripts() {
        for (auto it = m_Scripts.begin(); it != m_Scripts.end(); ++it) {
            if (it->expired())
                continue;
            BuildScript(it->lock());
        }

    }

    ByteBuffer ScriptEngine::SaveByteCode(Reference<Script> script)
    {
        if (!script->Module) {
            TRI_CORE_ERROR("Error saving bytecode: invalid script module");
            return {};
        }

        ByteBuffer buffer;
        ByteCodeStream stream(buffer);

        script->Module->SaveByteCode(&stream, true);

        return buffer;
    }

    void ScriptEngine::InstantiateScript(GameObject object)
    {
        TRI_CORE_ASSERT(!m_ScriptInstances.contains(static_cast<uint32_t>(object.GetHandle())), "A script instance already exists for this object");

        auto& sc = object.GetComponent<ScriptComponent>();

        if (!sc.ScriptResource->TypeInfo) {
            TRI_CORE_ERROR("Cannot instantiate script, see errors above");
            return;
        }

        asbind20::script_object scriptObject = asbind20::instantiate_class(m_Context, sc.ScriptResource->TypeInfo);
        // Prepare is called in asbind20::instantiate_class
        m_Context->Unprepare();

        m_ScriptInstances.emplace(static_cast<uint32_t>(object.GetHandle()), scriptObject);

        sc.Instance = &m_ScriptInstances.at(static_cast<uint32_t>(object.GetHandle()));
        EnumerateScript(sc.Instance);
    }

    void ScriptEngine::ClearScriptInstance(GameObject object) {
        if (!m_ScriptInstances.contains(static_cast<uint32_t>(object.GetHandle()))) {
            TRI_CORE_WARN("Script instances map doesn't contain key object '{}'", object.GetComponent<TagComponent>().Tag);
            return;
        }

        auto& sc = object.GetComponent<ScriptComponent>();
        sc.Instance->Clear();

        m_ScriptInstances.erase(static_cast<uint32_t>(object.GetHandle()));
    }

    void ScriptEngine::ClearAllScriptInstances(Scene* clearFrom)
    {
        TRI_CORE_ASSERT(clearFrom, "Tried to clear instances from invalid scene");
        for (const auto& [objectid, instance] : m_ScriptInstances) {
            GameObject object{static_cast<ObjectID>(objectid), clearFrom};
            auto& sc = object.GetComponent<ScriptComponent>();
            sc.Instance->Clear();
            sc.Instance = nullptr;
        }

        m_ScriptInstances.clear();
    }

    static ScriptVariableType StringToVariableType(std::string_view typeName) {
             if (typeName == "int8") return ScriptVariableType::Int8;
        else if (typeName == "int16") return ScriptVariableType::Int16;
        else if (typeName == "int" || typeName == "int32") return ScriptVariableType::Int32;
        else if (typeName == "int64") return ScriptVariableType::Int64;
        else if (typeName == "uint8") return ScriptVariableType::Uint8;
        else if (typeName == "uint16") return ScriptVariableType::Uint16;
        else if (typeName == "uint" || typeName == "uint32") return ScriptVariableType::Uint32;
        else if (typeName == "uint64") return ScriptVariableType::Uint64;
        else if (typeName == "bool") return ScriptVariableType::Bool;
        else if (typeName == "float") return ScriptVariableType::Float;
        else if (typeName == "double") return ScriptVariableType::Double;
        else if (typeName == "string") return ScriptVariableType::String;
        else if (typeName == "Float2") return ScriptVariableType::Vec2;
        else if (typeName == "Float3") return ScriptVariableType::Vec3;
        else if (typeName == "Float4") return ScriptVariableType::Vec4;
        else if (typeName == "GameObject") return ScriptVariableType::GameObject;
        else if (typeName == "Scene") return ScriptVariableType::Scene;
        else return ScriptVariableType::Unknown;

    }

    // asIScriptEngine::GetTypeInfoById returns nullptr if the typeid is a primitive type, hence the need for this function
    static ScriptVariableType GetPrimitiveType(int32_t typeID) {
        TRI_CORE_ASSERT(typeID <= asTYPEID_DOUBLE, "Not a primitive type");
        static constexpr std::array<ScriptVariableType, 12> type = {
            ScriptVariableType::Unknown, 
            ScriptVariableType::Bool, 
            ScriptVariableType::Int8, 
            ScriptVariableType::Int16, 
            ScriptVariableType::Int32, 
            ScriptVariableType::Int64, 
            ScriptVariableType::Uint8, 
            ScriptVariableType::Uint16, 
            ScriptVariableType::Uint32, 
            ScriptVariableType::Uint64, 
            ScriptVariableType::Float, 
            ScriptVariableType::Double
        };

        return type[typeID];
    }

    void ScriptEngine::EnumerateScript(ScriptInstance* script)
    {
        int32_t propertyCount = script->Object->GetPropertyCount();

        script->Properties.reserve(propertyCount);

        for (int32_t i = 0; i < propertyCount; i++) {
            std::string_view propertyName = script->Object->GetPropertyName(i);

            ScriptVariableType dataType;

            int32_t typeID = script->Object->GetPropertyTypeId(i);
            if (asbind20::is_primitive_type(typeID))
                dataType = GetPrimitiveType(typeID);
            else {
                asITypeInfo *typeInfo = m_Engine->GetTypeInfoById(typeID);
                TRI_CORE_ASSERT(typeInfo, "Invalid type info");
                dataType = StringToVariableType(typeInfo->GetName());
            }

            ScriptVariable property;
            property.Address = script->Object->GetAddressOfProperty(i);
            property.Type = dataType;

            script->Properties[propertyName.data()] = property;
        }
    }

    static void LineCallback(asIScriptContext* ctx, float* timeOut)
    {
        if (*timeOut < Time::GetTime())
            ctx->Suspend();
    }

    void ScriptEngine::ExecuteContext() {
        // Disabled because this causes a crash for some reason

        // float timeOut = Time::GetTime() + 2.0f;
        // m_Context->SetLineCallback(asFUNCTION(LineCallback), &timeOut, asCALL_CDECL);

        int32_t r = m_Context->Execute();
        if (r != asEXECUTION_FINISHED)
        {
            if (r == asEXECUTION_EXCEPTION)
            {
                auto* func = m_Context->GetExceptionFunction();
                TRI_CORE_ERROR("An exception '{}' occured executing a script", m_Context->GetExceptionString());
                TRI_CORE_ERROR("    in {} on line {}", func->GetDeclaration(), m_Context->GetExceptionLineNumber());
            }
        }

        m_Context->Unprepare();
    }

    void ScriptEngine::StartScript(GameObject object)
    {
        auto& sc = object.GetComponent<ScriptComponent>();
        if (!sc.ScriptResource->StartFunc || !sc.Instance)
            return;

        int32_t r;
        r = m_Context->Prepare(sc.ScriptResource->StartFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetObject(sc.Instance->Object.get());

        ExecuteContext();
    }

    void ScriptEngine::StopScript(GameObject object)
    {
        auto& sc = object.GetComponent<ScriptComponent>();
        if (!sc.ScriptResource->StopFunc || !sc.Instance)
            return;

        int32_t r;
        r = m_Context->Prepare(sc.ScriptResource->StopFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetObject(sc.Instance->Object.get());

        ExecuteContext();
    }

    void ScriptEngine::UpdateScript(GameObject object, float deltaTime)
    {
        auto& sc = object.GetComponent<ScriptComponent>();
        if (!sc.ScriptResource->UpdateFunc || !sc.Instance)
            return;

        int32_t r;
        r = m_Context->Prepare(sc.ScriptResource->UpdateFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetObject(sc.Instance->Object.get());
        m_Context->SetArgFloat(0, deltaTime);

        ExecuteContext();
    }

    void ScriptEngine::OnCollisionStart(GameObject object, GameObject collider)
    {
        auto& sc = object.GetComponent<ScriptComponent>();
        if (!sc.ScriptResource->CollisionStartFunc || !sc.Instance)
            return;
        
        int32_t r;
        r = m_Context->Prepare(sc.ScriptResource->CollisionStartFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetObject(sc.Instance->Object.get());
        m_Context->SetArgObject(0, &collider);

        ExecuteContext();
    }

    void ScriptEngine::OnCollisionStop(GameObject object, GameObject collider)
    {
        auto& sc = object.GetComponent<ScriptComponent>();
        if (!sc.ScriptResource->CollisionStopFunc || !sc.Instance)
            return;

        int32_t r;
        r = m_Context->Prepare(sc.ScriptResource->CollisionStopFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetObject(sc.Instance->Object.get());
        m_Context->SetArgObject(0, &collider);

        ExecuteContext();
    }

}