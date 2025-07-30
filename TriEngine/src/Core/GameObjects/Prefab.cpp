#include "tripch.h"

#include "Scene.h"
#include "GameObject.h"
#include "Components.h"

#include "Prefab.h"

namespace TriEngine {
    Prefab::Prefab(std::istream& data, entt::registry* registry)
        :m_Registry(registry)
    {

    }

    Prefab::Prefab(GameObject object, entt::registry* registry)
        :m_Registry(registry)
    {
    }

    void Prefab::Save(std::ostream& out)
    {
    }
}