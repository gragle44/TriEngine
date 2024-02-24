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
		~Scene();

		void OnUpdate(float deltaTime);
		void OnEditorViewportRender();
		void OnViewportResized(uint32_t width, uint32_t height);

		void SetMainCamera(GameObject& camera);
		GameObject& CreateSceneCamera(const std::string& tag = "Camera");
		GameObject CreateGameObject(const std::string& tag = std::string());

	private:
		friend class GameObject;

		void InitRender();
		void OnRender(float deltaTime);

		entt::registry m_Registry;

		std::unique_ptr<GameObject> m_CameraObject = nullptr;

		glm::vec2 m_ViewportSize;

		Reference<Renderpass> m_MainRenderpass;
	};
}