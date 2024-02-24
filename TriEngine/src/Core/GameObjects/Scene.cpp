#include "tripch.h"
#include "Scene.h"

#include "imgui.h"
#include "Components.h"
#include "GameObject.h"
#include "Script.h"

#include "Renderer/Renderer2D.h"

namespace TriEngine {
	Scene::Scene()
	{
		InitRender();
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(float deltaTime)
	{
		for (auto&& [entity, sc] : m_Registry.view<ScriptComponent>().each())
		{
			if (!sc.ScriptInstance)
			{
				sc.ScriptInstance = sc.InstantiateScript();
				sc.ScriptInstance->m_Object = GameObject(entity, this);
			}
			sc.ScriptInstance->OnUpdate(deltaTime);
		}


		OnRender(deltaTime);
	}

	void Scene::OnViewportResized(uint32_t width, uint32_t height)
	{
		m_ViewportSize = { width, height };
		if (m_MainRenderpass->Target != nullptr) {
			m_MainRenderpass->Target->ReSize(width, height);
		}
		auto view = m_Registry.view<Camera2DComponent>();

		for (auto entity : view) {
			auto& camera = view.get<Camera2DComponent>(entity);
			if (camera.Resizeable) {
				camera.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::InitRender()
	{
		FrameBufferSettings fbSettings;
		fbSettings.Width = 1280;
		fbSettings.Height = 720;

		Reference<FrameBuffer> mainFB = FrameBuffer::Create(fbSettings);

		m_MainRenderpass = std::make_shared<Renderpass>(mainFB);
	}

	void Scene::OnRender(float deltaTime)
	{
		//2D rendering
		Camera* camera = nullptr;
		glm::mat4 cameraTransform;

		if (m_CameraObject != nullptr) {
			cameraTransform = m_CameraObject->GetComponent<TransformComponent>().GetTransform();
			camera = (Camera*)&m_CameraObject->GetComponent<Camera2DComponent>().Camera;
		}

		if (camera) {
			Renderer2D::Begin(camera->GetProjection(), cameraTransform, m_MainRenderpass);

			auto group = m_Registry.group<TransformComponent>(entt::get<Sprite2DComponent>);

			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, Sprite2DComponent>(entity);

				Renderer2D::SubmitQuad({ .Transform = transform.GetTransform(), .Texture = sprite.Texture, .TilingFactor = sprite.TilingFactor, .Tint = sprite.Tint });
			}

			Renderer2D::End();
		}

		//TODO: Postprocess
	}

	void Scene::OnEditorRender()
	{
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		const Reference<Texture2D>& frameBufferTexture = m_MainRenderpass->Target->GetColorAttachment();
		ImGui::Image((void*)(intptr_t)frameBufferTexture->GetID(), viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	void Scene::SetMainCamera(GameObject camera)
	{
		if (!camera.HasComponent<Camera2DComponent>() || !camera.HasComponent<TransformComponent>()) {
			TRI_CORE_ERROR("Tried to set main camera to an object without a CameraComponent or a TransformComponent!");
			return;
		}
		m_CameraObject = std::make_unique<GameObject>(camera);
	}

	GameObject Scene::CreateGameObject(const std::string& tag)
	{
		GameObject object(m_Registry.create(), this);
		TagComponent& tagComponent = object.AddComponent<TagComponent>(tag);
		tagComponent.Tag = tag.empty() ? "Object" : tag;
		return object;
	}

	GameObject& Scene::CreateSceneCamera(const std::string& tag)
	{
		m_CameraObject = std::make_unique<GameObject>(CreateGameObject(tag));
		m_CameraObject->AddComponent<TransformComponent>();

		auto& cameraComponent = m_CameraObject->AddComponent<Camera2DComponent>();
		cameraComponent.Camera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		return *m_CameraObject;
	}
}