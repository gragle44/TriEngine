#pragma once

#include "Core/Resources/Resource.h"

#include "angelscript.h"

#include <string>

namespace TriEngine {
    struct ScriptBuild {
        asIScriptModule* Module = nullptr;

        asIScriptFunction* StartFunc = nullptr;
        asIScriptFunction* StopFunc = nullptr;
        asIScriptFunction* UpdateFunc = nullptr;
        asIScriptFunction* CollisionStartFunc = nullptr;
        asIScriptFunction* CollisionEndFunc = nullptr;
    };

    class Script : public Resource {
    public:
        Script() = default;
        virtual ~Script() = default;

        std::string Name;

        ScriptBuild Build;

        void ClearScriptBuild() {
            Build = {};
        }
    };
}