#pragma once

#include "Script.h"

#include "angelscript.h"

#include <filesystem>

namespace TriEngine {
    class ScriptEngine {
    public:
        ScriptEngine();
        ~ScriptEngine();

        void BuildScript(Reference<Script> script);
        Reference<Script> GetScript(const std::string& name) { return m_Scripts.at(name); }

        void StartScript(const Script& script);
        void StopScript(const Script& script);
        void UpdateScript(const Script& script, float deltaTime);

        const asIScriptEngine* GetASEngine() const { return m_Engine; }
    private:
        void ConfigureScriptEngine(asIScriptEngine* engine);

        std::unordered_map<std::string, Reference<Script>> m_Scripts;
        asIScriptEngine* m_Engine;
        asIScriptContext* m_Context;
    };
}