#include "Core/Resources/Resource.h"

#include "GameObject.h"

namespace TriEngine {
    class Prefab : public Resource {
    public:
        Prefab(std::istream& data, entt::registry* prefabRegistry);
        Prefab(GameObject object, entt::registry* prefabRegistry);
        void Save(std::ostream& out);

        template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Registry->emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent() {
			m_Registry->remove<T>(m_Handle);
		}

		template<typename T>
		[[nodiscard]] T& GetComponent() {
			return m_Registry->get<T>(m_Handle);
		}

		template<typename T>
		[[nodiscard]] bool HasComponent() const {
			return m_Registry->all_of<T>(m_Handle);
		}

        // TODO: move this into a utility somewhere
        template <typename... Cs>
        void CopyComponents(entt::registry& dst, entt::registry& src, entt::entity from, entt::entity to)
        {
            ([&]()
            {
            if(src.all_of<Cs>(from)) {
                dst.emplace_or_replace<Cs>(to, src.get<Cs>(from));
            } }(), ...);
        }

		[[nodiscard]] ObjectID GetHandle() const { return m_Handle; }
    private:
        entt::entity m_Handle = entt::null;
        entt::registry* m_Registry = nullptr;
    };
}