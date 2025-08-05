#include "Core/Resources/Resource.h"

#include "GameObject.h"

namespace TriEngine {
    class Prefab : public Resource {
    public:
        Prefab(std::istream& data);
        Prefab(GameObject object);

        void Save(std::ostream& out);

        [[nodiscard]] entt::entity GetRoot() const { return m_Root; }

		[[nodiscard]] entt::registry& GetRegistry() { return m_Registry; }

        [[nodiscard]] uint32_t GetEntityCount() const { return m_EntityCount; }
    private:
        void CopyFromObject(entt::entity entity, GameObject object);

        template <typename T>
        void CopyComponentFromGameObject(entt::entity entity, GameObject object) {
            if (object.HasComponent<T>())
                m_Registry.emplace_or_replace<T>(entity, object.GetComponent<T>());
        }

        void CopyAllComponents(entt::entity entity, GameObject object);

        entt::registry m_Registry;
        entt::entity m_Root = entt::null;
        uint32_t m_EntityCount = 0;
    };
}