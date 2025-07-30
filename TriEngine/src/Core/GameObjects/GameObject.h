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

		~GameObject() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Scene->m_Registry.emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent() {
			m_Scene->m_Registry.remove<T>(m_Handle);
		}

		template<typename T>
		[[nodiscard]] T& GetComponent() {
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		[[nodiscard]] bool HasComponent() const {
			return m_Scene->m_Registry.all_of<T>(m_Handle);
		}

		const std::string& GetName();
		UUID GetID();
		GameObject GetParent();

		[[nodiscard]] ObjectID GetHandle() const { return m_Handle; }

		[[nodiscard]] Scene* GetScene() const { return m_Scene; }

		operator bool() const noexcept { return m_Handle != entt::null; }

		bool operator==(const GameObject & other) const {
			return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
		}
		bool operator!=(const GameObject& other) const {
			return !(*this == other);
		}
	private:
		friend class NativeScript;

		entt::entity m_Handle = entt::null;
		Scene* m_Scene = nullptr;
	};
}
