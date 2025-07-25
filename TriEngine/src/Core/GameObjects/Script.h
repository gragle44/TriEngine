#pragma once

#include "GameObject.h"

namespace TriEngine {
	//Forward declared
	class Scene;

	class NativeScript {
	public:
		NativeScript() = default;
		virtual ~NativeScript() = default;

		virtual void OnStart() {}

		virtual void OnStop() {}

		virtual void OnUpdate(float deltaTime) {}

		virtual void OnCollisionStart(GameObject collider) {}
		virtual void OnCollisionEnd(GameObject collider) {}
	protected:
		template <typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Object.AddComponent<T>(std::forward<Args>(args)...);
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

		Scene* GetScene() { return m_Object.m_Scene; }

		friend class Scene;

		GameObject m_Object;
	};

	struct ScriptRegistry {
		using FactoryFunction = std::unique_ptr<NativeScript>(*)();

		static std::unordered_map<std::string, FactoryFunction>& Registry() {
			static std::unordered_map<std::string, FactoryFunction> registry;
			return registry;
		}

		template<typename T>
		static void Register() {
			std::string scriptName = typeid(T).name();
			FactoryFunction factoryFunction = []() -> std::unique_ptr<NativeScript> { return std::make_unique<T>(); };
			Registry().emplace(scriptName, factoryFunction);
		}

		static bool Exists(const std::string& scriptName) {
			return Registry().contains(scriptName);
		}

		static FactoryFunction GetFactoryFunction(const std::string& scriptName) {
			if (Exists(scriptName))
				return Registry().at(scriptName);
			return nullptr;
		}
	};
}