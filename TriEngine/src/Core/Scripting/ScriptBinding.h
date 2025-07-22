#pragma once

#include "angelscript.h"

#include "Core/GameObjects/GameObject.h"

namespace TriEngine {

    // Currently unused
    class ScriptInterface {
    public:
        virtual ~ScriptInterface() {}

        virtual void OnStart() {}
        virtual void OnStop() {}

        virtual void OnUpdate(float deltatime) {}

        virtual void OnCollisionStart(GameObject collider) {}
        virtual void OnCollisionStop(GameObject collider) {}
    };

    namespace Utils {
        void ConfigureScriptEngine(asIScriptEngine* engine);
    }
}