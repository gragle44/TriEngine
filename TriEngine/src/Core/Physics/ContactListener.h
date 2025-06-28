#pragma once

#include "Core/Scripting/ScriptEngine.h"

#include "box2d/b2_world_callbacks.h"

namespace TriEngine {
    class ContactListener : public b2ContactListener
    {
    public:
        ContactListener(ScriptEngine* scriptEngine)
            :m_ScriptEngine(scriptEngine) {}
    private:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);

        ScriptEngine* m_ScriptEngine;
    };
}

