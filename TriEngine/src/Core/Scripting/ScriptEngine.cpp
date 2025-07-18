#include "tripch.h"

#include "ScriptEngine.h"

#include "ScriptBinding.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Components.h"

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

    ScriptBuild ScriptEngine::BuildScript(GameObject object) 
    {
        auto& sc = object.GetComponent<ScriptComponent>();
        auto& script = sc.ScriptResource;

        TRI_CORE_ASSERT(script, "Invalid script resource")

        if (sc.Build.Module) {
            ClearScript(object);
            TRI_CORE_INFO("Rebuilding script {} for object {}", script->Name, object.GetComponent<TagComponent>().Tag);
        }
        TRI_CORE_ASSERT(sc.Build.Module == nullptr, "Attempted to build script that already has a built module");

        if (!std::filesystem::exists(script->MetaData.Filepath)) {
            TRI_CORE_WARN("Couldn't build script '{0}': invalid file path", script->MetaData.Filepath);
            return {};
        }
        std::string scriptName(std::to_string(static_cast<uint32_t>(object.GetHandle())));

        CScriptBuilder builder;
        int32_t r = builder.StartNewModule(m_Engine, scriptName.c_str()); 
        TRI_CORE_ASSERT(r >= 0, "Unrecoverable error while starting a new module");

        r = builder.AddSectionFromFile(script->MetaData.Filepath.c_str());
        if (r < 0) {
            TRI_CORE_ERROR("Couldn't build script {0}: see build errors", script->MetaData.Filepath);
            return {};
        }
        
        r = builder.BuildModule();
        if (r < 0) {
            TRI_CORE_ERROR("Couldn't build script {0}: see build errors", script->MetaData.Filepath);
            return {};
        }

        script->Name = scriptName;

        ScriptBuild build;
        build.Module = m_Engine->GetModule(scriptName.c_str());
        build.StartFunc = build.Module->GetFunctionByDecl("void on_start()");
        build.StopFunc = build.Module->GetFunctionByDecl("void on_stop()");
        build.UpdateFunc = build.Module->GetFunctionByDecl("void on_update(float)");
        build.CollisionStartFunc = build.Module->GetFunctionByDecl("void on_collision_start(GameObject)");
        build.CollisionStopFunc= build.Module->GetFunctionByDecl("void on_collision_stop(GameObject)");

        return build;
    }

    void ScriptEngine::ClearScript(GameObject object) {
        auto& sc = object.GetComponent<ScriptComponent>();

        if (sc.Build.Module) {
            sc.Build.Module->Discard();
        }

        sc.Build.Clear();
    }
    

    void ScriptEngine::ClearAllScripts()
    {
        int32_t moduleCount = m_Engine->GetModuleCount();

        for (int32_t i = 0; i < moduleCount-1; i++) {
            auto* module = m_Engine->GetModuleByIndex(i);
            if (module)
                module->Discard();
        }
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

    std::vector<ScriptVariable> ScriptEngine::GetScriptProperties(ScriptBuild build) {
        TRI_CORE_ASSERT(build.Module, "Invalid script module");

        int32_t globalVarCount = build.Module->GetGlobalVarCount();

        std::vector<ScriptVariable> properties;

        for (int32_t i = 0; i < globalVarCount; i++) {
            const char* name;
            const char* nameSpace;
            int32_t typeID;
            bool isConst;

            int32_t r = build.Module->GetGlobalVar(i, &name, &nameSpace, &typeID, &isConst);
            TRI_CORE_ASSERT(r >= 0, "Error getting global variable");

            // These global variables are reserved by the application
            if (name == "gameObject" || name == "scene")
                continue;

            ScriptVariableType dataType;

            if (asbind20::is_primitive_type(typeID))
                dataType = GetPrimitiveType(typeID);
            else {
                asITypeInfo* typeInfo = m_Engine->GetTypeInfoById(typeID);
                TRI_CORE_ASSERT(typeInfo, "Invalid type info");
                dataType = StringToVariableType(typeInfo->GetName());
            }

            ScriptVariable variable;
            variable.Name = name;
            variable.Address = build.Module->GetAddressOfGlobalVar(i);
            variable.Const = isConst;
            variable.Type = dataType;

            properties.push_back(std::move(variable));
        }

        return properties;
    }

    void ScriptEngine::StartScript(ScriptBuild build)
    {
        if (!build.StartFunc)
            return;

        //TODO: line callback

        int32_t r;
        r = m_Context->Prepare(build.StartFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        r = m_Context->Execute();
        if( r != asEXECUTION_FINISHED )
        {
            if( r == asEXECUTION_EXCEPTION )
            {
                TRI_CORE_ERROR("An exception '{0}' occurred. Please correct the code and try again.", m_Context->GetExceptionString());
            }
        }
        
        m_Context->Unprepare();
    }

    void ScriptEngine::StopScript(ScriptBuild build) 
    {
       if (!build.StopFunc)
            return;

        //TODO: line callback

        int32_t r;
        r = m_Context->Prepare(build.StopFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        r = m_Context->Execute();
        if( r != asEXECUTION_FINISHED )
        {
            if( r == asEXECUTION_EXCEPTION )
            {
                TRI_CORE_ERROR("An exception '{0}' occurred. Please correct the code and try again.", m_Context->GetExceptionString());
            }
        }
        
        m_Context->Unprepare();
    }

    void ScriptEngine::UpdateScript(ScriptBuild build, float deltaTime) 
    {
        if (!build.UpdateFunc)
            return;

        //TODO: line callback

        int32_t r;
        r = m_Context->Prepare(build.UpdateFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetArgFloat(0, deltaTime);

        r = m_Context->Execute();
        if( r != asEXECUTION_FINISHED )
        {
            if( r == asEXECUTION_EXCEPTION )
            {
                TRI_CORE_ERROR("An exception '{0}' occurred. Please correct the code and try again.", m_Context->GetExceptionString());
            }
        }

        m_Context->Unprepare();
    }

    void ScriptEngine::OnCollisionStart(ScriptBuild build, GameObject collider) 
    {
        if (!build.CollisionStartFunc)
            return;
        
        int32_t r;
        r = m_Context->Prepare(build.CollisionStartFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetArgObject(0, &collider);

        r = m_Context->Execute();
        if( r != asEXECUTION_FINISHED )
        {
            if( r == asEXECUTION_EXCEPTION )
            {
                TRI_CORE_ERROR("An exception '{0}' occurred. Please correct the code and try again.", m_Context->GetExceptionString());
            }
        }

        m_Context->Unprepare();
    }

    void ScriptEngine::OnCollisionStop(ScriptBuild build, GameObject collider) 
    {
        if (!build.CollisionStopFunc)
            return;
        
        int32_t r;
        r = m_Context->Prepare(build.CollisionStopFunc);
        TRI_CORE_ASSERT(r >= 0, "Failed to prepare the context");

        m_Context->SetArgObject(0, &collider);

        r = m_Context->Execute();
        if( r != asEXECUTION_FINISHED )
        {
            if( r == asEXECUTION_EXCEPTION )
            {
                TRI_CORE_ERROR("An exception '{0}' occurred. Please correct the code and try again.", m_Context->GetExceptionString());
            }
        }

        m_Context->Unprepare();
    }

}