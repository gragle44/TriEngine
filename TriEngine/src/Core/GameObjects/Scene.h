#pragma once

#include "EditorCamera.h"
#include "Core/Events/Event.h"
#include "Core/Renderer/RenderPass.h"
#include "Core/Physics/ContactListener.h"

#include "entt/entt.hpp"

// Forward declared
class b2World;

namespace TriEngine {
	class GameObject;
	class Prefab;
}

namespace TriEngine {
	class Scene : public Resource {
	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		static Reference<Scene> Create();
		static Reference<Scene> Create(const std::string& name);

		// Copies the data from other into the scene
		void Copy(Reference<Scene> other) { Copy(other.get()); }
		void Copy(Scene* other);

		void Start();
		void Stop();
		void Reset() { m_ShouldReset = true; }

		void OnEvent(Event& e);
		void OnEditorUpdate(float deltaTime);
		void OnUpdate(float deltaTime);
		void OnViewportResized(uint32_t width, uint32_t height);
		[[nodiscard]] const std::string& GetName() const { return m_Name; }
		void SetMainCamera(GameObject camera);

		[[nodiscard]] const std::unordered_map<uint64_t, GameObject>& GetAllObjects() const { return m_GameObjects; }

		[[nodiscard]] bool IsObjectValid(GameObject object);
		GameObject CreateGameObject(const std::string& tag = "");
		GameObject CreateGameObjectUUID(uint64_t uuid, const std::string& tag = "");

		Reference<Prefab> CreatePrefab(GameObject object) const;
		void InstantiatePrefab(Reference<Prefab> prefab);

		[[nodiscard]] GameObject GetObjectByID(UUID uuid) const noexcept;
		[[nodiscard]] GameObject GetObjectByName(const std::string& name) const noexcept;

		void OnObjectRenamed(GameObject object, std::string_view oldName, std::string_view newName);

		GameObject DuplicateObject(GameObject object);

		void AddChildToObject(GameObject object, GameObject child);

		void DeleteGameObject(GameObject object);

	private:
		friend class GameObject;
		friend class Prefab;
		friend class SceneModule;
		friend class GameRenderer;
		friend class SceneSerializer;

		void UpdateTransform(GameObject object, const glm::mat4 &parentTransform);

		void ShouldReset();

		std::string IncrementObjectName(const std::string& name);

		std::string m_Name;

		entt::registry m_Registry;
		std::unordered_map<uint64_t, GameObject> m_GameObjects;
		std::unordered_map<std::string, GameObject> m_GameObjectNameMapping;

		static entt::registry s_PrefabRegistry;

		// Pointer to avoid including GameObject.h
		GameObject* m_DummyObject = nullptr;

		std::unique_ptr<Scene> m_ResetPoint = nullptr;

		b2World* m_PhysicsWorld = nullptr;
		ContactListener* m_ContactListener = nullptr;

		// Needed to restore cameras when the scene resets
		glm::ivec2 m_ViewPortSize;

		bool m_ShouldReset = false;
	};
}