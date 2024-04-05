#pragma once

#include "EditorCamera.h"
#include "Core/Events/Event.h"
#include "Core/Renderer/RenderPass.h"
#include "entt/entt.hpp"

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

		void OnEvent(Event& e);
		void OnUpdate(float deltaTime);
		void OnEditorRender();
		void OnViewportResized(uint32_t width, uint32_t height);
		const std::string& GetName() const { return m_Name; }

		void SetEditorCamera(Reference<EditorCamera> camera);
		void SetMainCamera(GameObject camera);
		GameObject CreateGameObject(const std::string& tag = std::string());
		void DeleteGameObject(GameObject object);

	private:
		friend class GameObject;
		friend class SceneModule;
		friend class SceneSerializer;


		void InitRender();
		void OnRender(float deltaTime);

		std::string m_Name;
		entt::registry m_Registry;


		Reference<EditorCamera> m_CameraObject = nullptr;

		glm::vec2 m_ViewportSize;

		Reference<Renderpass> m_MainRenderpass;
	};
}