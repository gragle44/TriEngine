#pragma once

#include "GameObject.h"

namespace TriEngine {
	//Forward declared
	class Scene;

	class Script {
	public:
		Script() = default;
		virtual ~Script() = default;

		virtual void OnUpdate(float deltaTime) {}
	protected:
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Object.AddComponent<T>(args);
		}

		template<typename T>
		T& GetComponent() {
			return m_Object.GetComponent<T>();
		}

		template<typename T>
		void RemoveComponent() {
			m_Object.RemoveComponent<T>();
		}

		template<typename T>
		bool ComponentExists() {
			return m_Object.HasComponent<T>();
		}

		friend class Scene;

		GameObject m_Object;
	};
}