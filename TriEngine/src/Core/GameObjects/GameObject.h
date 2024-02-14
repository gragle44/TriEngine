#pragma once

#include "Scene.h"
#include "entt/entt.hpp"

namespace TriEngine {
	using ObjectID = entt::entity;

	class GameObject {
	public:
		GameObject() = default;
		GameObject(entt::entity handle, Scene* scene)
			:m_Handle(handle), m_Scene(scene) {};
		GameObject(const GameObject& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent() {
			m_Scene->m_Registry.remove<T>(m_Handle);
		}

		template<typename T>
		T& GetComponent() {
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_Handle);
		}

		ObjectID GetHandle() const { return m_Handle; }

		operator bool() const { return m_Handle != entt::null; }
	private:
		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
