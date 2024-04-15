#pragma once

#include "nameof/nameof.hpp"

#include "GameObject.h"

namespace TriEngine {
	//Forward declared
	class Scene;

	class Script {
	public:
		Script() = default;
		virtual ~Script() = default;

		virtual void OnCreate() {}

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

		Scene* GetScene() { return m_Object.m_Scene; }

		friend class Scene;

		GameObject m_Object;
	};

	struct ScriptRegistry {
		using FactoryFunction = std::unique_ptr<Script>(*)();

		static std::unordered_map<std::string, FactoryFunction>& Registry() {
			static std::unordered_map<std::string, FactoryFunction> registry;
			return registry;
		}

		template<typename T>
		static void Register() {
			std::string scriptName = std::string(NAMEOF_TYPE(T));
			FactoryFunction factoryFunction = []() -> std::unique_ptr<Script> { return std::make_unique<T>(); };
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