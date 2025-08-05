#pragma once

#include "box2d/b2_world_callbacks.h"

namespace TriEngine {
    class Scene;

    class ContactListener : public b2ContactListener
    {
    public:
        ContactListener(Scene* scene)
            :m_Scene(scene) {}
    private:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);

        Scene* m_Scene;
    };
}

