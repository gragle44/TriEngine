#pragma once

#include "Script.h"

#include "angelscript.h"

namespace TriEngine {
    class ByteCodeStream : public asIBinaryStream {
    public:
        ByteCodeStream(ByteBuffer& buffer)
            :m_Buffer(buffer) {}
        ~ByteCodeStream() override = default;

        int Write(const void *ptr, uint32_t size) override;
        int Read(void *ptr, uint32_t size) override;
    private:
        ByteBuffer& m_Buffer;
        uint32_t m_CurrentPos = 0;
    };

    class ScriptEngine {
    public:
        static ScriptEngine& Get();

        ScriptEngine(const ScriptEngine&) = delete;
        void operator=(const ScriptEngine&) = delete;

        ~ScriptEngine();

        // Build a script module
        [[nodiscard]] ScriptBuild BuildScript(GameObject object);
        // Compile a script into bytecode
        void CompileScript(Script* script);
        void ClearScript(GameObject object);
        void ClearAllScripts();

        [[nodiscard]] std::vector<ScriptVariable> GetScriptProperties(ScriptBuild build);

        void StartScript(ScriptBuild build);
        void StopScript(ScriptBuild build);
        void UpdateScript(ScriptBuild build, float deltaTime);
        void OnCollisionStart(ScriptBuild build, GameObject collider);
        void OnCollisionStop(ScriptBuild build, GameObject collider);

        template<typename T>
        void SetGlobalVariable(const ScriptBuild& build, std::string_view decl, T value) 
        {
            if (!build.Module) {
                TRI_CORE_WARN("Tried to set a global variable on a script without a valid script module");
                return;
            }
                
            int32_t varIndex = build.Module->GetGlobalVarIndexByDecl(decl.data());
            if (varIndex < 0)
                return;

            T* var = reinterpret_cast<T*>(build.Module->GetAddressOfGlobalVar(varIndex));  
            *var = value;
        }

        [[nodiscard]] const asIScriptEngine* GetASEngine() const { return m_Engine; }
    private:
        ScriptEngine();

        void ConfigureScriptEngine(asIScriptEngine* engine);

        asIScriptEngine* m_Engine;
        asIScriptContext* m_Context;
    };
}