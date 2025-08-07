#include "tripch.h"
#include "Scene.h"

#include "Core/Base/UUID.h"
#include "Components.h"
#include "GameObject.h"
#include "Prefab.h"
#include "Script.h"

#include "Core/Base/Input.h"

#include "Core/Scripting/ScriptEngine.h"

#include "Core/Physics/ContactListener.h"

#include "Renderer/Renderer2D.h"
#include "Renderer/Particle.h"
#include "Renderer/RenderCommand.h"

#include "imgui.h"

#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

#include <execution>

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

    template <typename T>
    static void CopyComponent(entt::entity to, entt::registry& toReg, entt::entity from, entt::registry& fromReg) {
        if (fromReg.all_of<T>(from))
            toReg.emplace_or_replace<T>(to, fromReg.get<T>(from));
    }

    static void CopyAllComponents(entt::entity to, entt::registry& toReg, entt::entity from, entt::registry& fromReg) {
        CopyComponent<PrefabComponent>(to, toReg, from, fromReg);
        CopyComponent<RelationshipComponent>(to, toReg, from, fromReg);
        CopyComponent<Transform2DComponent>(to, toReg, from, fromReg);
        CopyComponent<TransformComponent>(to, toReg, from, fromReg);
        CopyComponent<RigidBody2DComponent>(to, toReg, from, fromReg);
        CopyComponent<BoxCollider2DComponent>(to, toReg, from, fromReg);
        CopyComponent<NativeScriptComponent>(to, toReg, from, fromReg);
        CopyComponent<ScriptComponent>(to, toReg, from, fromReg);
        CopyComponent<Camera2DComponent>(to, toReg, from, fromReg);
        CopyComponent<Sprite2DComponent>(to, toReg, from, fromReg);
        CopyComponent<ParticleEmmiterComponent>(to, toReg, from, fromReg);
    }

    static void CopyAllComponents(GameObject newObject, GameObject oldObject) {
		CopyAllComponents(newObject.GetHandle(), newObject.GetScene()->GetRegistry(), oldObject.GetHandle(), oldObject.GetScene()->GetRegistry());
    }

	static void AddChildAsFixture(GameObject object, const glm::vec2& parentPosition, float parentRotation, b2Body* parentBody) {
		if (!object.HasComponent<BoxCollider2DComponent>())
			return;

		const auto& collider = object.GetComponent<BoxCollider2DComponent>();
		const auto& transform = object.GetComponent<Transform2DComponent>();
		const auto& relationship = object.GetComponent<RelationshipComponent>();
		b2PolygonShape box;

		b2Vec2 b2parentPosition = {parentPosition.x, parentPosition.y};
		box.SetAsBox(collider.Size.x * abs(transform.Scale.x), collider.Size.y * abs(transform.Scale.y), b2parentPosition, glm::radians(parentRotation));

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &box;
		fixtureDef.density = collider.Density;
		fixtureDef.friction = collider.Friction;
		fixtureDef.restitution = collider.Restitution;
		fixtureDef.restitutionThreshold = collider.RestitutionThreshold;

		parentBody->CreateFixture(&fixtureDef);

		for (UUID childid : relationship.Children)
		{
			glm::vec2 position = (parentPosition + glm::vec2{transform.Position.x, transform.Position.y});
			float rotation = parentRotation + transform.Rotation;
			AddChildAsFixture(object.GetScene()->GetObjectByID(childid), position, rotation, parentBody);
		}
    }

	void Scene::InitializeGameObject(GameObject object) {
		if (object.HasComponent<RigidBody2DComponent>() && object.HasComponent<BoxCollider2DComponent>()) {
			auto& rigidBody = object.GetComponent<RigidBody2DComponent>();
			auto& collider = object.GetComponent<BoxCollider2DComponent>();
			auto& transform = object.GetComponent<Transform2DComponent>();
			const auto& relationship = object.GetComponent<RelationshipComponent>();

			// TODO: let children have their own rigid bodies
			if (!relationship.Parent) {
				b2BodyDef bodyDef;
				bodyDef.type = BodyTypeToB2Type(rigidBody.Type);
				bodyDef.position.Set(transform.Position.x, transform.Position.y);
				bodyDef.angle = glm::radians(transform.Rotation);
				bodyDef.userData.pointer = static_cast<uintptr_t>(object.GetHandle());

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

				for (UUID childid : relationship.Children) {
					AddChildAsFixture(GetObjectByID(childid), {transform.Position.x, transform.Position.y}, transform.Rotation, body);
				}
			}
		}

		if (object.HasComponent<ScriptComponent>()) {
            ScriptEngine& scriptEngine = ScriptEngine::Get();
            auto& sc = object.GetComponent<ScriptComponent>();

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
        m_PhysicsWorld = std::make_unique<b2World>(b2Vec2{0.0f, -10.0f});

        m_ContactListener = std::make_unique<ContactListener>(this);

		m_PhysicsWorld->SetContactListener(m_ContactListener.get());

		ScriptEngine& scriptEngine = ScriptEngine::Get();
		scriptEngine.RebuildAllScripts();

		for (auto entity : m_Registry.view<entt::entity>()) {
			InitializeGameObject({entity, this});
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

		m_PhysicsWorld.reset();
		m_ContactListener.reset();

		//m_Registry.clear();
		//m_GameObjects.clear();
		//m_GameObjectNameMapping.clear();
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

	void Scene::UpdateTransform(GameObject object, const glm::mat4& parentTransform)
    {
		TRI_PROFILE;
		auto& transform = object.GetComponent<TransformComponent>();
		const auto& localTransform = object.GetComponent<Transform2DComponent>();
		const auto& relationship = object.GetComponent<RelationshipComponent>();

		glm::mat4 localMatrix = localTransform.GetTransform();
		transform.Transform = localMatrix * parentTransform;

		for (auto childID : relationship.Children) {
			GameObject child = GetObjectByID(childID);
			UpdateTransform(child, transform.Transform);
		}
    }

	void Scene::OnUpdate(float deltaTime)
	{
		TRI_PROFILE;

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

		{
			TRI_PROFILE_NAMED("Update Scripts");
			for (auto&& [entity, sc] : m_Registry.view<ScriptComponent>().each())
			{
				ScriptEngine& scriptEngine = ScriptEngine::Get();

				if (sc.Active && sc.ScriptResource) {
					scriptEngine.UpdateScript({ entity, this }, deltaTime);
				}
			}
		}

		{
			TRI_PROFILE_NAMED("Update Physics");

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
		}

		{
			TRI_PROFILE_NAMED("Update Transforms");

			auto transformView = m_Registry.view<Transform2DComponent, TransformComponent, RelationshipComponent>();

			for (auto [entity, transform2d, transform, relationship] : transformView.each()) {
				if (relationship.Parent == static_cast<UUID>(0)) {
					UpdateTransform({ entity, this }, glm::mat4(1.0f));
				}
			}
		}

		{
			TRI_PROFILE_NAMED("Update Particles");

			auto particleView = m_Registry.view<ParticleEmmiterComponent, Transform2DComponent>();

			ParticleSystem::GetData().ShouldRender = false;
			ParticleSystem::GetData().ParticleBuffer->Bind(0);
			ParticleSystem::GetData().FreelistBuffer->Bind(1);

			// Particle spawn updates

			// TODO: when creating particles, calculate their global transform

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
		}

		ShouldReset();
	}

	void Scene::OnEditorUpdate(float deltaTime) {
		auto view = m_Registry.view<Transform2DComponent, TransformComponent, RelationshipComponent>();

		for (auto [entity, transform2d, transform, relationship] : view.each())
		{
			if (relationship.Parent == static_cast<UUID>(0))
			{
				UpdateTransform({entity, this}, glm::mat4(1.0f));
			}
		}
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

	void Scene::AddChildToObject(GameObject object, GameObject child)
	{
		child.GetComponent<RelationshipComponent>().Parent = object.GetComponent<IDComponent>().ID;

		object.GetComponent<RelationshipComponent>().Children.push_back(child.GetComponent<IDComponent>().ID);
	}

	GameObject Scene::DuplicateObject(GameObject object)
	{
		TRI_CORE_ASSERT(IsObjectValid(object), "Tried to duplicate an invalid object");
		GameObject newObject = CreateGameObject(object.GetComponent<TagComponent>().Tag);

		const auto& relationship = object.GetComponent<RelationshipComponent>();

		CopyAllComponents(newObject, object);

		// Replaces the copied relationship component
		auto& newRelationship = newObject.AddComponent<RelationshipComponent>(relationship.Parent);

		for (UUID childid : relationship.Children) {
			GameObject newChild = DuplicateObject(GetObjectByID(childid));
			newRelationship.Children.push_back(newChild.GetID());
		}

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

    GameObject Scene::CreateGameObject(entt::entity entity) {
        GameObject object(entity, this);

		// This is the only component that isnt serialized, so it has to be added here
		object.AddComponent<TransformComponent>();

        UUID uuid = object.GetComponent<IDComponent>().ID;
        const std::string& name = object.GetComponent<TagComponent>().Tag;

        m_GameObjects.emplace(uuid, object);
        m_GameObjectNameMapping.emplace(name, object);
		return object;
    }

    GameObject Scene::CreateGameObject(const std::string& tag) {
        return CreateGameObjectUUID(UUID(), tag);
    }

    std::string Scene::IncrementObjectName(const std::string& name)
	{
		for (uint32_t i = 1;; i++)
		{
            // TODO: increment based on find_last_not_of("0123456789");
        	std::string candidate = name + std::to_string(i);
			if (!m_GameObjectNameMapping.contains(candidate))
				return candidate;
		}
	}

	GameObject Scene::CreateGameObjectUUID(uint64_t uuid, const std::string& tag)
	{
		GameObject object(m_Registry.create(), this);
		object.AddComponent<IDComponent>(uuid);

		object.AddComponent<RelationshipComponent>();
		TRI_CORE_ASSERT(uuid == object.GetComponent<IDComponent>().ID, "UUIDs did not match when creating object");

		TagComponent& tagComponent = object.AddComponent<TagComponent>(tag);
		tagComponent.Tag = tag.empty() ? "Object" : tag;

		if (m_GameObjectNameMapping.contains(tagComponent.Tag))
			tagComponent.Tag = IncrementObjectName(tagComponent.Tag);

		object.AddComponent<Transform2DComponent>();
		object.AddComponent<TransformComponent>();

		m_GameObjects.emplace(uuid, object);
		m_GameObjectNameMapping.emplace(tagComponent.Tag, object);
		return object;
	}

    Reference<Prefab> Scene::CreatePrefab(GameObject object) const {
		return std::make_shared<Prefab>(object);
    }

    GameObject Scene::InstantiatePrefab(Reference<Prefab> prefab) {
        GameObject rootObject;

		std::unordered_map<UUID, GameObject> prefabToObject;

		prefabToObject.reserve(prefab->GetEntityCount());

		entt::registry& prefabRegistry = prefab->GetRegistry();
        for (auto entity : prefabRegistry.view<entt::entity>()) {
            auto instance = m_Registry.create();
            GameObject object(instance, this);

            CopyAllComponents(instance, m_Registry, entity, prefabRegistry);

            object.AddComponent<IDComponent>(UUID());
            object.AddComponent<PrefabComponent>();
            object.AddComponent<TransformComponent>();

            prefabToObject[prefabRegistry.get<IDComponent>(entity).ID] = object;

			m_GameObjects.emplace(object.GetID(), object);

            if (!object.HasParent())
                rootObject = object;
        }

        TRI_CORE_ASSERT(rootObject, "No root object found in prefab");

		auto view = prefab->GetRegistry().view<RelationshipComponent>();
		TRI_CORE_ASSERT(view.size() == prefabToObject.size(), "Every entity in a prefab should have a RelationshipComponent");
		for (const auto&& [entity, relationship] : view.each()) {
			UUID prefabID = prefabRegistry.get<IDComponent>(entity).ID;
			GameObject object = prefabToObject.at(prefabID);
			
			auto& newRelationship = object.GetComponent<RelationshipComponent>();

            if (object.HasParent()) {
                newRelationship.Parent = prefabToObject[relationship.Parent].GetID();
            }

			for (size_t i = 0; i < newRelationship.Children.size(); i++) {
				newRelationship.Children[i] = prefabToObject[relationship.Children[i]].GetID();
			}
        }

		for (auto& [_, object] : prefabToObject) {
			InitializeGameObject(object);
		}

        return rootObject;
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

		if (object.HasComponent<RelationshipComponent>()) {
			auto& relationship = object.GetComponent<RelationshipComponent>();
			if (relationship.Parent) {
				GameObject parent = m_GameObjects.at(relationship.Parent);
				auto& parentChildren = parent.GetComponent<RelationshipComponent>().Children;
				parentChildren.erase(std::find(parentChildren.begin(), parentChildren.end(), UUID(uuid)));
			}

			for (UUID childid : relationship.Children) {
				GameObject child = GetObjectByID(childid);
				DeleteGameObject(child);
			}
		}

		if (object.HasComponent<ScriptComponent>()) {
			ScriptEngine& engine = ScriptEngine::Get();
			engine.ClearScriptInstance(object);
		}

		m_GameObjectNameMapping.erase(name);
		m_GameObjects.erase(uuid);
		m_Registry.destroy(object.GetHandle());
	}
}