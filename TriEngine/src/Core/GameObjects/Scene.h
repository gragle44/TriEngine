#pragma once

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

		void OnUpdate(float deltaTime);
		void OnEditorRender();
		void OnViewportResized(uint32_t width, uint32_t height);

		void SetMainCamera(GameObject camera);
		GameObject& CreateSceneCamera(const std::string& tag = "Camera");
		GameObject CreateGameObject(const std::string& tag = std::string());
		void DeleteGameObject(GameObject object);

	private:
		friend class GameObject;
		friend class SceneModule;

		void InitRender();
		void OnRender(float deltaTime);

		std::string m_Name;
		entt::registry m_Registry;

		std::unique_ptr<GameObject> m_CameraObject = nullptr;

		glm::vec2 m_ViewportSize;

		Reference<Renderpass> m_MainRenderpass;
	};
}