#include "tripch.h"
#include "Scene.h"

#include "imgui.h"
#include "Core/Base/UUID.h"
#include "Components.h"
#include "GameObject.h"
#include "Script.h"
#include "Core/Base/Input.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_weld_joint.h"

#include "Renderer/Renderer2D.h"
#include "Renderer/Particle.h"
#include "Renderer/RenderCommand.h"

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
		MetaData.Type = ResourceType::Scene;
	}

	Scene::Scene(const std::string& name)
		:m_Name(name)
	{
		MetaData.Type = ResourceType::Scene;
	}

	Scene::~Scene()
	{
		delete m_PhysicsWorld;
		delete m_ContactListener;
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
		CopyComponent<RelationshipComponent>(newObject, oldObject);
		CopyComponent<Transform2DComponent>(newObject, oldObject);
		CopyComponent<RigidBody2DComponent>(newObject, oldObject);
		CopyComponent<BoxCollider2DComponent>(newObject, oldObject);
		CopyComponent<NativeScriptComponent>(newObject, oldObject);
		CopyComponent<ScriptComponent>(newObject, oldObject);
		CopyComponent<Camera2DComponent>(newObject, oldObject);
		CopyComponent<Sprite2DComponent>(newObject, oldObject);
		CopyComponent<ParticleEmmiterComponent>(newObject, oldObject);
	}

	void Scene::Start()
	{
		m_ResetPoint = Copy();

		auto cameraView = m_Registry.view<Camera2DComponent>();

		for (auto entity : cameraView) {
			auto& camera = cameraView.get<Camera2DComponent>(entity);
			if (camera.Resizable) {
				camera.Camera.SetViewportSize(m_ViewPortSize.x, m_ViewPortSize.y);
			}
		}

		//TODO: Adjustable gravity in project settings
		m_PhysicsWorld = new b2World({ 0.0f, -10.0f });

		m_ContactListener = new ContactListener();

		m_PhysicsWorld->SetContactListener(m_ContactListener);

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
			bodyDef.userData.pointer = (uintptr_t)&m_GameObjects.at(object.GetComponent<IDComponent>().ID);

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

		/*
		// Link child nodes to their parents
		for (auto entity : view) {
			GameObject object = { entity, this };

			auto& rigidBody = object.GetComponent<RigidBody2DComponent>();
			const auto& transform = object.GetComponent<Transform2DComponent>();
			const auto& relationship = object.GetComponent<RelationshipComponent>();

			if (!relationship.Parent)
				continue;

			GameObject parent = GetObjectByID(relationship.Parent);

			if (!parent.HasComponent<RigidBody2DComponent>() || !parent.HasComponent<BoxCollider2DComponent>()) 
				continue;

			auto& parentRigidBody = parent.GetComponent<RigidBody2DComponent>();
			const auto& parentTransform = parent.GetComponent<Transform2DComponent>();
		
			b2Body* parentBody = reinterpret_cast<b2Body*>(parentRigidBody.Body);
			b2Body* body = reinterpret_cast<b2Body*>(rigidBody.Body);

			b2WeldJointDef jointDef;
			jointDef.Initialize(parentBody, body, parentBody->GetLocalPoint(parentBody->GetWorldCenter()));

			m_PhysicsWorld->CreateJoint(&jointDef);
		}
		*/

		for (auto&& [entity, sc] : m_Registry.view<ScriptComponent>().each())
		{

			GameObject object(entity, this);

			ScriptEngine& scriptEngine = ScriptEngine::Get();

			sc.Build = scriptEngine.BuildScript(object);

			if (sc.ScriptInstance) {
				scriptEngine.SetGlobalVariable<Scene*>(sc.Build, "Scene@ scene", this);
				scriptEngine.SetGlobalVariable<GameObject>(sc.Build, "GameObject gameObject", object);
				scriptEngine.StartScript(sc.Build);
			}
		}
		
	}

	void Scene::Stop()
	{
		for (auto&& [entity, sc] : m_Registry.view<NativeScriptComponent>().each())
		{
			if (sc.ScriptActive && sc.ScriptInstance) {
				if (sc.ScriptInstance)
				{
					sc.ScriptInstance->OnStop();
				}
				sc.ScriptInstance = nullptr;
			}
		}

		ScriptEngine& scriptEngine = ScriptEngine::Get();

		for (auto&& [entity, sc] : m_Registry.view<ScriptComponent>().each())
		{
			if (sc.Active && sc.ScriptInstance) {
				scriptEngine.StopScript(sc.Build);
			}
			sc.Build.Clear();
		}

		scriptEngine.ClearScripts();

		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;

		delete m_ContactListener;
		m_ContactListener = nullptr;

		m_Registry.clear();
		m_GameObjects.clear();
	}

	void Scene::Reset() 
	{
		m_ShouldReset = true;
	}

	void Scene::ShouldReset()
	{
		if (m_ShouldReset) {
			m_ShouldReset = false;
			Stop();

			auto idView = m_ResetPoint->m_Registry.view<IDComponent>();

			for (auto entity : idView) {
				GameObject object = { entity, m_ResetPoint.get() };

				const std::string& name = object.GetComponent<TagComponent>().Tag;

				auto id = object.GetComponent<IDComponent>().ID;

				GameObject newObject = CreateGameObjectUUID(id, name);

				CopyAllComponents(newObject, object);
			}

			Start();
		}
	}

	void Scene::OnUpdate(float deltaTime)
	{
		for (auto&& [entity, sc] : m_Registry.view<NativeScriptComponent>().each())
		{
			if (sc.ScriptActive && sc.InstantiateScript != nullptr) {
				if (!sc.ScriptInstance)
				{
					sc.ScriptInstance = sc.InstantiateScript();
					sc.ScriptInstance->m_Object = GameObject(entity, this);
					sc.ScriptInstance->OnStart();
				}
				sc.ScriptInstance->OnUpdate(deltaTime);
			}
		}

		for (auto&& [entity, sc] : m_Registry.view<ScriptComponent>().each())
		{
			ScriptEngine& scriptEngine = ScriptEngine::Get();

			if (sc.Active && sc.ScriptInstance) {
				scriptEngine.UpdateScript(sc.Build, deltaTime);
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

		auto particleView = m_Registry.view<ParticleEmmiterComponent, Transform2DComponent>();

		ParticleSystem::GetData().ShouldRender = false;
		ParticleSystem::GetData().ParticleBuffer->Bind(0);
		ParticleSystem::GetData().FreelistBuffer->Bind(1);

		// Particle spawn updates
		ParticleSystem::GetData().EmmisionShader->Bind();

		for (auto entity : particleView) {
			GameObject object = { entity, this };

			auto& emmiter = object.GetComponent<ParticleEmmiterComponent>();
			auto& transform = object.GetComponent<Transform2DComponent>();

			ParticleSystem::UpdateEmmitter(deltaTime, emmiter, transform);
		}

		RenderCommand::MemoryBarrier();

		// Particle Updates

		ParticleSystem::GetData().UpdateShader->Bind();

		for (auto entity : particleView) {
			GameObject object = { entity, this };

			auto& emmiter = object.GetComponent<ParticleEmmiterComponent>();
			auto& transform = object.GetComponent<Transform2DComponent>();

			ParticleSystem::Update(deltaTime);
		}
		RenderCommand::MemoryBarrier();

		ShouldReset();
	}

	void Scene::OnViewportResized(uint32_t width, uint32_t height)
	{
		m_ViewPortSize = { width, height };

		auto view = m_Registry.view<Camera2DComponent>();

		for (auto entity : view) {
			auto& camera = view.get<Camera2DComponent>(entity);
			if (camera.Resizable) {
				camera.Camera.SetViewportSize(width, height);
			}
		}
	}

	void Scene::OnEvent(Event& e)
	{

	}

	GameObject Scene::DuplicateObject(GameObject object)
	{
		TRI_CORE_ASSERT(IsObjectValid(object), "Tried to duplicate an invalid object");
		GameObject newObject = CreateGameObject(object.GetComponent<TagComponent>().Tag);

		CopyAllComponents(newObject, object);

		return newObject;
	}

	void Scene::SetMainCamera(GameObject camera)
	{
		if (!camera.HasComponent<Camera2DComponent>() || !camera.HasComponent<Transform2DComponent>()) {
			TRI_CORE_ERROR("Tried to set main camera to an object without a CameraComponent or a TransformComponent!");
			return;
		}
	}

	bool Scene::IsObjectValid(GameObject object)
	{
		return m_Registry.valid(object.GetHandle());
	}

	GameObject Scene::CreateGameObject(const std::string& tag)
	{
		return CreateGameObjectUUID(UUID(), tag);
	}

	Reference<Scene> Scene::Copy()
	{
		Reference<Scene> newScene = Scene::Create(m_Name);

		newScene->MetaData = MetaData;

		auto idView = m_Registry.view<IDComponent>();

		for (auto entity : idView) {
			GameObject object = { entity, this };

			const std::string& name = object.GetComponent<TagComponent>().Tag;

			auto id = object.GetComponent<IDComponent>().ID;

			GameObject newObject = newScene->CreateGameObjectUUID(id, name);

			CopyAllComponents(newObject, object);
		}

		return newScene;
	}

	GameObject Scene::CreateGameObjectUUID(uint64_t uuid, const std::string& tag)
	{
		GameObject object(m_Registry.create(), this);
		object.AddComponent<IDComponent>(uuid);

		TRI_CORE_ASSERT(uuid == object.GetComponent<IDComponent>().ID, "UUIDs did not match when creating object");

		TagComponent& tagComponent = object.AddComponent<TagComponent>(tag);
		tagComponent.Tag = tag.empty() ? "Object" : tag;

		object.AddComponent<Transform2DComponent>();

		object.AddComponent<RelationshipComponent>();

		m_GameObjects.emplace(uuid, object);
		return object;
	}

	GameObject Scene::GetObjectByID(UUID uuid) const noexcept {
		if (m_GameObjects.contains(uuid))
			return m_GameObjects.at(uuid);
		TRI_CORE_ERROR("Could not get game object, invalid uuid: {}", uuid);
		return {};
	}


	void Scene::DeleteGameObject(GameObject object)
	{
		uint64_t uuid = object.GetComponent<IDComponent>().ID;

		//TODO: erase references to this object in its relationships

		m_GameObjects.erase(uuid);
		m_Registry.destroy(object.GetHandle());
	}
}