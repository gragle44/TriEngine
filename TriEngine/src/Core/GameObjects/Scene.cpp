#include "tripch.h"
#include "Scene.h"

#include "imgui.h"
#include "Components.h"
#include "GameObject.h"
#include "Script.h"

#include "Renderer/Renderer2D.h"

namespace TriEngine {
	Scene::Scene()
		:m_Name("Scene")
	{
		InitRender();
	}

	Scene::Scene(const std::string& name)
		:m_Name(name)
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
			if (sc.ScriptActive) {
				if (!sc.ScriptInstance)
				{
					sc.ScriptInstance = sc.InstantiateScript();
					sc.ScriptInstance->m_Object = GameObject(entity, this);
				}
				sc.ScriptInstance->OnUpdate(deltaTime);
			}
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
		fbSettings.Samples = 1;

		Reference<FrameBuffer> mainFB = FrameBuffer::Create(fbSettings);

		m_MainRenderpass = std::make_shared<Renderpass>(mainFB);
	}

	void Scene::OnRender(float deltaTime)
	{
		//2D rendering
		Camera* camera = nullptr;
		glm::mat4 cameraTransform;

		if (m_CameraObject != nullptr) {
			cameraTransform = m_CameraObject->GetComponent<Transform2DComponent>().GetTransform();
			camera = (Camera*)&m_CameraObject->GetComponent<Camera2DComponent>().Camera;
		}

		//TODO: provide a default runtime camera and editor camera if the user does not supply one
		if (camera) {
			Renderer2D::Begin(camera->GetProjection(), cameraTransform, m_MainRenderpass);

			auto group = m_Registry.group<Transform2DComponent>(entt::get<Sprite2DComponent>);

			for (auto entity : group) {
				auto [transform, sprite] = group.get<Transform2DComponent, Sprite2DComponent>(entity);

				bool empty = sprite.Texture->GetData().empty();
				if (!empty) {
					Renderer2D::SubmitQuad({ .Transform = transform.GetTransform(), .Tint = sprite.Tint, .Texture = sprite.Texture, .TilingFactor = sprite.TilingFactor });
				}
				else {
					Renderer2D::SubmitQuad(ColoredQuadn(transform.GetTransform(), sprite.Tint, sprite.TilingFactor));
				}

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
		if (!camera.HasComponent<Camera2DComponent>() || !camera.HasComponent<Transform2DComponent>()) {
			TRI_CORE_ERROR("Tried to set main camera to an object without a CameraComponent or a TransformComponent!");
			return;
		}
		m_CameraObject = std::make_unique<GameObject>(camera);
	}

	GameObject Scene::CreateGameObject(const std::string& tag)
	{
		//TRI_CORE_INFO("Creating object {0} in {1}", tag, m_Name);
		GameObject object(m_Registry.create(), this);
		TagComponent& tagComponent = object.AddComponent<TagComponent>(tag);
		tagComponent.Tag = tag.empty() ? "Object" : tag;
		object.AddComponent<Transform2DComponent>();
		return object;
	}

	void Scene::DeleteGameObject(GameObject object)
	{
		std::string& name = object.GetComponent<TagComponent>().Tag;
		if (m_CameraObject && object == *m_CameraObject)
			m_CameraObject = nullptr;
		m_Registry.destroy(object.GetHandle());
	}

	GameObject& Scene::CreateSceneCamera(const std::string& tag)
	{
		m_CameraObject = std::make_unique<GameObject>(CreateGameObject(tag));

		auto& cameraComponent = m_CameraObject->AddComponent<Camera2DComponent>();
		cameraComponent.Camera.SetViewportSize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		return *m_CameraObject;
	}
}