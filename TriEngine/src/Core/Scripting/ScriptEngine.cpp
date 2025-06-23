#include "tripch.h"

#include "ScriptEngine.h"
#include "ScriptBinding.h"

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
    ScriptEngine::ScriptEngine()
        :m_Engine(nullptr)
    {
        m_Engine = asCreateScriptEngine();
        TRI_CORE_ASSERT(m_Engine != nullptr, "Failed to create script engine");

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

    void ScriptEngine::BuildScript(Reference<Script> script) 
    {
        std::string scriptName = std::filesystem::path(script->MetaData.Filepath).filename();

        if (m_Scripts.contains(scriptName)) {
            TRI_CORE_TRACE("Tried to build a script that has already been built: {0}", scriptName);
        }

        CScriptBuilder builder;
        int32_t r = builder.StartNewModule(m_Engine, scriptName.c_str()); 
        TRI_CORE_ASSERT(r >= 0, "Unrecoverable error while starting a new module");

        r = builder.AddSectionFromFile(script->MetaData.Filepath.c_str());
        TRI_CORE_ASSERT(r >= 0, "Please correct the errors in the script and try again");

        r = builder.BuildModule();
        TRI_CORE_ASSERT(r >= 0, "Please correct the errors in the script and try again");

        script->Name = scriptName;
        script->Build.Module = m_Engine->GetModule(scriptName.c_str());
        script->Build.StartFunc = script->Build.Module->GetFunctionByDecl("void on_start()");
        script->Build.StopFunc = script->Build.Module->GetFunctionByDecl("void on_stop()");
        script->Build.UpdateFunc = script->Build.Module->GetFunctionByDecl("void on_update(float)");
        // TODO: collision functions

        m_Scripts[scriptName] = script;
    }

    void ScriptEngine::StartScript(const Script& script)
    {
        if (!script.Build.StartFunc)
            return;

        //TODO: line callback

        int32_t r;
        r = m_Context->Prepare(script.Build.StartFunc);
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

    void ScriptEngine::StopScript(const Script& script) 
    {
       if (!script.Build.StopFunc)
            return;

        //TODO: line callback

        int32_t r;
        r = m_Context->Prepare(script.Build.StopFunc);
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

    void ScriptEngine::UpdateScript(const Script& script, float deltaTime) 
    {
        if (!script.Build.UpdateFunc)
            return;

        //TODO: line callback

        int32_t r;
        r = m_Context->Prepare(script.Build.UpdateFunc);
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

}