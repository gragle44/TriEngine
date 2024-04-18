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
}

namespace TriEngine {
	class Scene {
	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		static Reference<Scene> Create();
		static Reference<Scene> Create(const std::string& name);

		Reference<Scene> Copy();

		void Start();
		void Stop();
		void Reset();

		void OnEvent(Event& e);
		void OnUpdate(float deltaTime);
		void OnEditorUpdate(float deltaTime);
		void OnEditorRender();
		void OnViewportResized(uint32_t width, uint32_t height);
		const std::string& GetName() const { return m_Name; }

		void SetEditorCamera(Reference<EditorCamera> camera);
		void SetMainCamera(GameObject camera);

		const std::unordered_map<uint64_t, GameObject>& GetAllObjects() const { return m_GameObjcts; }

		GameObject CreateGameObject(const std::string& tag = std::string());
		GameObject CreateGameObjectUUID(uint64_t uuid, const std::string& tag = std::string());
		GameObject DuplicateObject(GameObject object);
		void DeleteGameObject(GameObject object);

	private:
		friend class GameObject;
		friend class SceneModule;
		friend class SceneSerializer;

		void InitRender();
		void OnRender(float deltaTime);

		void ShouldReset();


		std::string m_Name;

		entt::registry m_Registry;
		std::unordered_map<uint64_t, GameObject> m_GameObjcts;

		Reference<Scene> m_ResetPoint = nullptr;

		b2World* m_PhysicsWorld = nullptr;
		ContactListener* m_ContactListener = nullptr;

		Reference<EditorCamera> m_CameraObject = nullptr;

		glm::vec2 m_ViewportSize;

		bool m_ShouldReset = false;

		Reference<Renderpass> m_MainRenderpass;
	};
}