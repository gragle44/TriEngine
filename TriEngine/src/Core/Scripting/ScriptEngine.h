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

    class Scene;

    class ScriptEngine {
    public:
        static ScriptEngine& Get();

        ScriptEngine(const ScriptEngine&) = delete;
        void operator=(const ScriptEngine&) = delete;

        ~ScriptEngine();

        void BuildScript(Reference<Script> script);
        void RebuildAllScripts();

        ByteBuffer SaveByteCode(Reference<Script> script);

        void InstantiateScript(GameObject object);

        void ClearScriptInstance(GameObject object);
        void ClearAllScriptInstances(Scene* clearFrom);

        void StartScript(GameObject object);
        void StopScript(GameObject object);
        void UpdateScript(GameObject object, float deltaTime);
        void OnCollisionStart(GameObject object, GameObject collider);
        void OnCollisionStop(GameObject object, GameObject collider);

        template<typename T>
        void SetScriptProperty(const ScriptInstance* instance, const std::string& name, T value) 
        {
            if (!instance)
                return;
            if (!instance->Properties.contains(name)) {
                TRI_CORE_WARN("Script doesn't contain property '{}'", name);
                return;
            }
                
            auto& property = instance->Properties.at(name);
            T* var = reinterpret_cast<T*>(property.Address);  
            *var = value;
        }

        [[nodiscard]] const asIScriptEngine* GetASEngine() const { return m_Engine; }
    private:
        ScriptEngine();

        void EnumerateScript(ScriptInstance* script);

        void ExecuteContext();
        void ConfigureScriptEngine(asIScriptEngine* engine);

        // Mapping of Object ids to script instances
        std::unordered_map<uint32_t, ScriptInstance> m_ScriptInstances;

        std::vector<std::weak_ptr<Script>> m_Scripts;

        asIScriptEngine* m_Engine;
        asIScriptContext* m_Context;
    };
}