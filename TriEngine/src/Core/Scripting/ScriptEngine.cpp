#include "tripch.h"

#include "ScriptEngine.h"

#include "ScriptBinding.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Components.h"

#include "scriptbuilder/scriptbuilder.h"

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
        auto& script = object.GetComponent<ScriptComponent>().ScriptInstance;

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

    void ScriptEngine::ClearScripts()
    {
        int32_t moduleCount = m_Engine->GetModuleCount();

        for (int32_t i = 0; i < moduleCount-1; i++) {
            auto* module = m_Engine->GetModuleByIndex(i);
            if (module)
                module->Discard();
        }
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