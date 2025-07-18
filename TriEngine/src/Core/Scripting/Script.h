#pragma once

#include "Core/Resources/Resource.h"

#include "angelscript.h"

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
        Vec4
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
        std::string Name;
        void* Address;
        bool Const;
        ScriptVariableType Type;
    };

    struct ScriptBuild {
        asIScriptModule* Module = nullptr;

        asIScriptFunction* StartFunc = nullptr;
        asIScriptFunction* StopFunc = nullptr;
        asIScriptFunction* UpdateFunc = nullptr;
        asIScriptFunction* CollisionStartFunc = nullptr;
        asIScriptFunction* CollisionStopFunc = nullptr;

        operator bool() const noexcept {
            return Module != nullptr;
        }

        void Clear() {
            *this = {};
        }
    };

    class Script : public Resource {
    public:
        Script() = default;
        Script(const Script& other)
            :Name(other.Name), Resource(other) {}

        virtual ~Script() = default;

        std::string Name;
    };
}