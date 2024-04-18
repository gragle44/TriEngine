#pragma once

#include "box2d/b2_world_callbacks.h"

namespace TriEngine {
    class ContactListener : public b2ContactListener
    {
    private:
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
    };
}

