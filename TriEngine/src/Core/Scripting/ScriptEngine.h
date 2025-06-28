#pragma once

#include "Script.h"

#include "angelscript.h"

#include <filesystem>

namespace TriEngine {
    class ScriptEngine {
    public:
        ScriptEngine();
        ~ScriptEngine();

        ScriptBuild BuildScript(Reference<Script> script, GameObject object);

        void StartScript(ScriptBuild build);
        void StopScript(ScriptBuild build);
        void UpdateScript(ScriptBuild build, float deltaTime);
        void OnCollisionStart(ScriptBuild build, GameObject collider);
        void OnCollisionStop(ScriptBuild build, GameObject collider);

        template<typename T>
        void SetGlobalVariable(const ScriptBuild& build, std::string_view decl, T value) 
        {
            if (!build.Module)
                return;
                
            int32_t varIndex = build.Module->GetGlobalVarIndexByDecl(decl.data());
            if (varIndex < 0)
                return;

            T* var = reinterpret_cast<T*>(build.Module->GetAddressOfGlobalVar(varIndex));  
            *var = value;
        }

        const asIScriptEngine* GetASEngine() const { return m_Engine; }
    private:
        void ConfigureScriptEngine(asIScriptEngine* engine);

        asIScriptEngine* m_Engine;
        asIScriptContext* m_Context;
    };
}