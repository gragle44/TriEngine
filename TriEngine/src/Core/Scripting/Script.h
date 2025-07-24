#pragma once

#include "Core/Resources/Resource.h"

#include "angelscript.h"
#include "asbind20/asbind.hpp"

#include <string>

namespace TriEngine {

    class GameObject;

    enum class ScriptVariableType : uint8_t {
        Unknown = 0,
        Int8,
        Int16,
        Int32,
        Int64,
        Uint8,
        Uint16,
        Uint32,
        Uint64,
        Float,
        Double,
        Bool,
        String,
        Vec2,
        Vec3,
        Vec4,
        GameObject,
        Scene
    };

    namespace Utils {
        constexpr bool IsScriptVariableTypeScalar(ScriptVariableType t) noexcept
        {
            switch (t) {
                case ScriptVariableType::Int8:
                case ScriptVariableType::Int16:
                case ScriptVariableType::Int32:
                case ScriptVariableType::Int64:
                case ScriptVariableType::Uint8:
                case ScriptVariableType::Uint16:
                case ScriptVariableType::Uint32:
                case ScriptVariableType::Uint64:
                case ScriptVariableType::Float:
                case ScriptVariableType::Double:
                    return true;
                default:
                    return false;
            }
        }
    }

    struct ScriptVariable {
        void* Address;
        ScriptVariableType Type;
    };

    struct ScriptInstance {
        ScriptInstance() = default;
        ScriptInstance(asbind20::script_object& object)
            : Object(std::move(object)) {}

        operator bool() const noexcept
        {
            return Object.get() != nullptr;
        }

        void Clear()
        {
            Properties = {};
            Object.reset();
        }

        asbind20::script_object Object;
        std::unordered_map<std::string, ScriptVariable> Properties;
    };

    class Script : public Resource {
    public:
        Script() = default;
        Script(const Script& other) = default;

        virtual ~Script() = default;

        std::string Name;

        // Will be empty when running in the editor
        ByteBuffer Bytecode;

        asIScriptModule* Module = nullptr;

        asITypeInfo* TypeInfo = nullptr;

        asIScriptFunction* StartFunc = nullptr;
        asIScriptFunction* StopFunc = nullptr;
        asIScriptFunction* UpdateFunc = nullptr;
        asIScriptFunction* CollisionStartFunc = nullptr;
        asIScriptFunction* CollisionStopFunc = nullptr;

        bool HasDebugInfo = true;
    };
}