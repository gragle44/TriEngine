#include "tripch.h"
#include "Scene.h"

#include "imgui.h"
#include "Core/Base/UUID.h"
#include "Components.h"
#include "GameObject.h"
#include "Script.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include "Renderer/Renderer2D.h"

namespace TriEngine {

	static b2BodyType BodyTypeToB2Type(RigidBody2DComponent::BodyType type) {
		switch (type)
		{
		case TriEngine::RigidBody2DComponent::BodyType::Static: return b2_staticBody;
		case TriEngine::RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
		case TriEngine::RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		default: return b2BodyType::b2_staticBody;
		}
	}

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
		delete m_PhysicsWorld;
	}

	Reference<Scene> Scene::Create()
	{
		return std::make_shared<Scene>();
	}

	Reference<Scene> Scene::Create(const std::string& name)
	{
		return std::make_shared<Scene>(name);
	}

	template<typename T>
	static void CopyComponent(GameObject newObject, GameObject oldObject) {
		if (oldObject.HasComponent<T>())
			newObject.AddComponent<T>(oldObject.GetComponent<T>());
	}

	static void CopyAllComponents(GameObject newObject, GameObject oldObject) {
		CopyComponent<Transform2DComponent>(newObject, oldObject);
		CopyComponent<RigidBody2DComponent>(newObject, oldObject);
		CopyComponent<BoxCollider2DComponent>(newObject, oldObject);
		CopyComponent<ScriptComponent>(newObject, oldObject);
		CopyComponent<Camera2DComponent>(newObject, oldObject);
		CopyComponent<Sprite2DComponent>(newObject, oldObject);
	}

	void Scene::Start()
	{
		m_ResetPoint = Copy();

		//TODO: Adjustable gravity in project settings
		m_PhysicsWorld = new b2World({ 0.0f, -10.0f });

		auto view = m_Registry.view<RigidBody2DComponent, BoxCollider2DComponent>();

		for (auto entity : view) {
			GameObject object = { entity, this };

			auto& rigidBody = object.GetComponent<RigidBody2DComponent>();
			auto& collider = object.GetComponent<BoxCollider2DComponent>();
			auto& transform = object.GetComponent<Transform2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = BodyTypeToB2Type(rigidBody.Type);
			bodyDef.position.Set(transform.Position.x, transform.Position.y);
			bodyDef.angle = glm::radians(transform.Rotation);

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);

			rigidBody.Body = body;

			b2PolygonShape box;

			box.SetAsBox(collider.Size.x * abs(transform.Scale.x), collider.Size.y * abs(transform.Scale.y));
				
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &box;
			fixtureDef.density = collider.Density;
			fixtureDef.friction = collider.Friction;
			fixtureDef.restitution = collider.Restitution;
			fixtureDef.restitutionThreshold = collider.RestitutionThreshold;

			body->CreateFixture(&fixtureDef);

		}
	}

	void Scene::Stop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void Scene::Reset()
	{
		m_Registry.clear();
		Stop();
		//TRI_CORE_TRACE("Reseting scene {0}", m_Name);
		auto idView = m_ResetPoint->m_Registry.view<IDComponent>();

		for (auto entity : idView) {
			GameObject object = { entity, m_ResetPoint.get()};

			std::string name = object.GetComponent<TagComponent>().Tag;

			GameObject newObject = CreateGameObject(name);

			CopyAllComponents(newObject, object);
		}

		Start();
	}

	void Scene::OnUpdate(float deltaTime)
	{
		for (auto&& [entity, sc] : m_Registry.view<ScriptComponent>().each())
		{
			if (sc.ScriptActive && sc.InstantiateScript != nullptr) {
				if (!sc.ScriptInstance)
				{
					sc.ScriptInstance = sc.InstantiateScript();
					sc.ScriptInstance->m_Object = GameObject(entity, this);
					sc.ScriptInstance->OnCreate();
				}
				sc.ScriptInstance->OnUpdate(deltaTime);
			}
		}

		//Physics config
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;

		m_PhysicsWorld->Step(deltaTime, velocityIterations, positionIterations);

		auto view = m_Registry.view<RigidBody2DComponent, BoxCollider2DComponent>();

		for (auto entity : view) {
			GameObject object = { entity, this };

			auto& rigidBody = object.GetComponent<RigidBody2DComponent>();
			auto& transform = object.GetComponent<Transform2DComponent>();

			b2Body* body = (b2Body*)rigidBody.Body;
			const b2Vec2& position = body->GetPosition();
			transform.Position.x = position.x;
			transform.Position.y = position.y;
			transform.Rotation = glm::degrees(body->GetAngle());

		}

		OnRender(deltaTime);
	}

	void Scene::OnEditorUpdate(float deltaTime)
	{
		OnRender(deltaTime);
	}

	void Scene::OnViewportResized(uint32_t width, uint32_t height)
	{
		m_ViewportSize = { width, height };
		if (m_MainRenderpass->Target != nullptr) {
			m_MainRenderpass->Target->ReSize(width, height);
		}

		if (m_CameraObject)
			m_CameraObject->SetViewportSize(width, height);

		auto view = m_Registry.view<Camera2DComponent>();

		for (auto entity : view) {
			auto& camera = view.get<Camera2DComponent>(entity);
			if (camera.Resizeable) {
				camera.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::SetEditorCamera(Reference<EditorCamera> camera)
	{
		m_CameraObject = camera;
	}

	void Scene::OnEvent(Event& e)
	{

	}

	void Scene::InitRender()
	{
		FrameBufferSettings fbSettings;
		fbSettings.Width = 1280;
		fbSettings.Height = 720;
		fbSettings.Samples = 1;
		fbSettings.Attachments = { RenderAttachmentType::Color };

		Reference<FrameBuffer> mainFB = FrameBuffer::Create(fbSettings);

		m_MainRenderpass = std::make_shared<Renderpass>(mainFB);
		m_MainRenderpass->DepthTest = false;
	}

	void Scene::OnRender(float deltaTime)
	{
		//2D rendering
		glm::mat4 cameraTransform;

		if (m_CameraObject != nullptr) {
			m_CameraObject->OnUpdate(deltaTime);
			cameraTransform = m_CameraObject->GetTransform();
			Renderer2D::Begin(m_CameraObject->GetProjection(), cameraTransform, m_MainRenderpass);

			auto group = m_Registry.group<Transform2DComponent>(entt::get<Sprite2DComponent>);

			for (auto entity : group) {
				auto [transform, sprite] = group.get<Transform2DComponent, Sprite2DComponent>(entity);

				bool empty = sprite.Texture->GetData().empty();
				if (!empty) {
					Renderer2D::SubmitQuad({ .Transform = transform.GetTransform(), .Tint = sprite.Tint, .Texture = sprite.Texture, .TilingFactor = sprite.TilingFactor, .Transparent = sprite.HasTransparency()});
				}
				else {
					Renderer2D::SubmitQuad(ColoredQuadn(transform.GetTransform(), sprite.Tint, sprite.TilingFactor, sprite.HasTransparency()));
				}

			}

			Renderer2D::End();
		}

		//TODO: Postprocess
	}


	GameObject Scene::DuplicateObject(GameObject object)
	{

		GameObject newObject = CreateGameObject(object.GetComponent<TagComponent>().Tag);

		CopyAllComponents(newObject, object);

		return newObject;
	}

	void Scene::OnEditorRender()
	{
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();

		ImGui::Image((void*)(intptr_t)m_MainRenderpass->Target->GetColorAttachment(0), viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	}

	void Scene::SetMainCamera(GameObject camera)
	{
		if (!camera.HasComponent<Camera2DComponent>() || !camera.HasComponent<Transform2DComponent>()) {
			TRI_CORE_ERROR("Tried to set main camera to an object without a CameraComponent or a TransformComponent!");
			return;
		}
	}

	GameObject Scene::CreateGameObject(const std::string& tag)
	{
		return CreateGameObjectUUID(UUID(), tag);
	}

	Reference<Scene> Scene::Copy()
	{
		Reference<Scene> newScene = Scene::Create(m_Name);

		newScene->m_CameraObject = m_CameraObject;
		newScene->m_ViewportSize = m_ViewportSize;
		newScene->m_MainRenderpass = m_MainRenderpass;

		auto idView = m_Registry.view<IDComponent>();

		for (auto entity : idView) {
			GameObject object = { entity, this };

			std::string name = object.GetComponent<TagComponent>().Tag;

			GameObject newObject = newScene->CreateGameObject(name);

			CopyAllComponents(newObject, object);
		}

		return newScene;
	}

	GameObject Scene::CreateGameObjectUUID(uint64_t uuid, const std::string& tag)
	{
		GameObject object(m_Registry.create(), this);
		IDComponent idComponent = object.AddComponent<IDComponent>(uuid);
		TagComponent& tagComponent = object.AddComponent<TagComponent>(tag);
		tagComponent.Tag = tag.empty() ? "Object" : tag;
		object.AddComponent<Transform2DComponent>();
		return object;
	}

	void Scene::DeleteGameObject(GameObject object)
	{
		std::string& name = object.GetComponent<TagComponent>().Tag;
		m_Registry.destroy(object.GetHandle());
	}

}