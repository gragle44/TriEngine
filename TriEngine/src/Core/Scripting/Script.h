#pragma once

#include "Core/Resources/Resource.h"

#include "angelscript.h"

#include <string>

namespace TriEngine {

    class GameObject;

    struct ScriptBuild {
        asIScriptModule* Module = nullptr;

        asIScriptFunction* StartFunc = nullptr;
        asIScriptFunction* StopFunc = nullptr;
        asIScriptFunction* UpdateFunc = nullptr;
        asIScriptFunction* CollisionStartFunc = nullptr;
        asIScriptFunction* CollisionStopFunc = nullptr;

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