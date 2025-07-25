#include "tripch.h"
#include "Scene.h"

#include "Core/Base/UUID.h"
#include "Components.h"
#include "GameObject.h"
#include "Script.h"

#include "Core/Base/Input.h"

#include "Core/Scripting/ScriptEngine.h"

#include "Renderer/Renderer2D.h"
#include "Renderer/Particle.h"
#include "Renderer/RenderCommand.h"

#include "imgui.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

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
		m_ResetPoint = std::make_unique<Scene>();
		m_ResetPoint->Copy(this);

		CreateGameObjectUUID(0, "Invalid Object");
		m_DummyObject = &m_GameObjects.at(0);

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

		ScriptEngine& scriptEngine = ScriptEngine::Get();

		scriptEngine.RebuildAllScripts();

		for (auto&& [entity, sc] : m_Registry.view<ScriptComponent>().each())
		{
			GameObject object(entity, this);

			// Clear the instance if it has been built in the editor already
			if (sc.Instance)
				scriptEngine.ClearScriptInstance(object);

			if (sc.ScriptResource) {
				scriptEngine.InstantiateScript(object);
				scriptEngine.SetScriptProperty<Scene*>(sc.Instance, "scene", this);
				scriptEngine.SetScriptProperty<GameObject>(sc.Instance, "gameObject", object);
				scriptEngine.StartScript(object);
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
			GameObject object {entity, this};
			if (sc.Active && sc.ScriptResource) {
				scriptEngine.StopScript(object);
			}
			scriptEngine.ClearScriptInstance(object);
		}

		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;

		delete m_ContactListener;
		m_ContactListener = nullptr;

		//m_Registry.clear();
		//m_GameObjects.clear();
		//m_GameObjectNameMapping.clear();
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

			Copy(m_ResetPoint.get());

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

			if (sc.Active && sc.ScriptResource) {
				scriptEngine.UpdateScript({entity, this}, deltaTime);
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
		TRI_CORE_ASSERT(false, "Not implemented");
	}

	bool Scene::IsObjectValid(GameObject object)
	{
		return m_Registry.valid(object.GetHandle());
	}

	GameObject Scene::CreateGameObject(const std::string& tag)
	{
		return CreateGameObjectUUID(UUID(), tag);
	}

	void Scene::Copy(Scene* other)
	{
		TRI_CORE_ASSERT(other, "Scene was null");

		if (!m_GameObjects.empty()) {
			m_Registry.clear();
			m_GameObjects.clear();
			m_GameObjectNameMapping.clear();
		}

		m_Name = other->m_Name;
		MetaData = other->MetaData;

		auto idView = other->m_Registry.view<IDComponent>();

		for (auto entity : idView) {
			GameObject object = { entity, other };

			const std::string& name = object.GetComponent<TagComponent>().Tag;

			auto id = object.GetComponent<IDComponent>().ID;

			GameObject newObject = CreateGameObjectUUID(id, name);

			CopyAllComponents(newObject, object);
		}
	}

	std::string Scene::IncrementObjectName(const std::string& name)
	{
		for (uint32_t i = 1;; i++)
		{
			std::string candidate = name + std::to_string(i);
			if (!m_GameObjectNameMapping.contains(candidate))
				return candidate;
		}
	}

	GameObject Scene::CreateGameObjectUUID(uint64_t uuid, const std::string& tag)
	{
		GameObject object(m_Registry.create(), this);
		object.AddComponent<IDComponent>(uuid);

		TRI_CORE_ASSERT(uuid == object.GetComponent<IDComponent>().ID, "UUIDs did not match when creating object");

		TagComponent& tagComponent = object.AddComponent<TagComponent>(tag);
		tagComponent.Tag = tag.empty() ? "Object" : tag;

		if (m_GameObjectNameMapping.contains(tagComponent.Tag))
			tagComponent.Tag = IncrementObjectName(tagComponent.Tag);

		object.AddComponent<Transform2DComponent>();

		m_GameObjects.emplace(uuid, object);
		m_GameObjectNameMapping.emplace(tagComponent.Tag, object);
		return object;
	}

	GameObject Scene::GetObjectByID(UUID uuid) const noexcept {
		TRI_CORE_ASSERT(m_GameObjects.contains(uuid), "Invalid UUID");
		return m_GameObjects.at(uuid);
	}

    GameObject Scene::GetObjectByName(const std::string& name) const noexcept
    {
		if (m_GameObjectNameMapping.contains(name))
			return m_GameObjectNameMapping.at(name);
		TRI_CORE_WARN("Can't get game object: unknown name '{}'", name);
		return *m_DummyObject;
	}

    void Scene::OnObjectRenamed(GameObject object, std::string_view oldName, std::string_view newName)
    {
		auto node = m_GameObjectNameMapping.extract(oldName.data());
		node.key() = newName;
		m_GameObjectNameMapping.insert(std::move(node));
	}

    void Scene::DeleteGameObject(GameObject object)
	{
		TRI_CORE_ASSERT(IsObjectValid(object), "Tried to delete invalid game object");
		uint64_t uuid = object.GetComponent<IDComponent>().ID;
		const std::string& name = object.GetComponent<TagComponent>().Tag;

		if (object.HasComponent<ScriptComponent>()) {
			ScriptEngine& engine = ScriptEngine::Get();
			engine.ClearScriptInstance(object);
		}

		m_GameObjectNameMapping.erase(name);
		m_GameObjects.erase(uuid);
		m_Registry.destroy(object.GetHandle());
	}
}