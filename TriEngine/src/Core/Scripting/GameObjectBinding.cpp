#include "tripch.h"

#include "GameObjectBinding.h"

#include "Core/GameObjects/Components.h"
#include "Core/GameObjects/GameObject.h"
#include "Core/GameObjects/Scene.h"

#include "asbind20/asbind.hpp"

namespace TriEngine {

    static void SetVelocity_Proxy(RigidBody2DComponent* this_, float x, float y) {
        this_->SetVelocity({x, y});
    }

    static void SetTransform_Proxy(RigidBody2DComponent* this_, glm::vec2 position, float angle) {
        b2Body* body = (b2Body*)this_->Body;

        body->SetTransform({position.x, position.y}, angle);
    }

    static const std::string& GetScriptName_Proxy(ScriptComponent* this_) {
        return this_->ScriptResource->Name;
    }

    static void BindComponents(asIScriptEngine* engine) {
        asbind20::ref_class<Transform2DComponent>(
            engine,
            "Transform2DComponent",
            asOBJ_NOCOUNT
        )
            .method("Mat4 GetTransform() const", &Transform2DComponent::GetTransform)
            .property("Float3 Position", &Transform2DComponent::Position)
            .property("float Rotation", offsetof(Transform2DComponent, Rotation))
            .property("Float2 Scale", offsetof(Transform2DComponent, Scale));

        asbind20::ref_class<TagComponent>(
            engine,
            "TagComponent",
            asOBJ_NOCOUNT
        )
            .property("string Tag", &TagComponent::Tag);

        asbind20::ref_class<IDComponent>(
            engine,
            "IDComponent",
            asOBJ_NOCOUNT
        )
            .property("uint64 ID", &IDComponent::ID);

        asbind20::ref_class<ScriptComponent>(
            engine,
            "ScriptComponent",
            asOBJ_NOCOUNT
        )
            .property("bool Active", offsetof(ScriptComponent, Active))
            .method("const string& GetName() const", &GetScriptName_Proxy);

        asbind20::ref_class<Camera2DComponent>(
            engine,
            "Camera2DComponent",
            asOBJ_NOCOUNT
        )
            .property("OrthographicCamera Camera", &Camera2DComponent::Camera)
            .property("bool Primary", asOFFSET(Camera2DComponent, Primary))
            .property("bool Resizable", asOFFSET(Camera2DComponent, Resizable));

        asbind20::ref_class<Sprite2DComponent>(
            engine,
            "Sprite2DComponent",
            asOBJ_NOCOUNT
        )
            .property("Float4 Tint", offsetof(Sprite2DComponent, Tint))
            .property("float TilingFactor", offsetof(Sprite2DComponent, TilingFactor));

        asbind20::enum_<RigidBody2DComponent::BodyType>(engine, "RigidBodyType")
            .value(RigidBody2DComponent::BodyType::Dynamic, "Dynamic")
            .value(RigidBody2DComponent::BodyType::Kinematic, "Kinematic")
            .value(RigidBody2DComponent::BodyType::Static, "Static");

        asbind20::ref_class<RigidBody2DComponent>(
            engine,
            "RigidBody2DComponent",
            asOBJ_NOCOUNT
        )
            .method("void ApplyVelocity(const Float2& in)", &RigidBody2DComponent::ApplyVelocity)
            .method("void SetVelocity(float x, float y)", &SetVelocity_Proxy)
            .method("void SetPosition(const Float2& in)", &RigidBody2DComponent::SetPosition)
            .method("void SetAngularVelocity(float velocityRadians)", &RigidBody2DComponent::SetAngularVelocity)
            .method("void ApplyAngularImpulse(float impulse, bool wake)", &RigidBody2DComponent::ApplyAngularImpulse)
            .method("void SetTransform(Float2 position, float angle)", &SetTransform_Proxy)
            .method("void SetPosition(Float2 position)", &RigidBody2DComponent::SetPosition)
            .property("RigidBodyType Type", &RigidBody2DComponent::Type);

        asbind20::ref_class<BoxCollider2DComponent>(
            engine,
            "BoxCollider2DComponent",
            asOBJ_NOCOUNT
        )
            .property("Float2 Size", &BoxCollider2DComponent::Size)
            .property("float Density", offsetof(BoxCollider2DComponent, Density))
            .property("float Friction", offsetof(BoxCollider2DComponent, Friction))
            .property("float Restitution", offsetof(BoxCollider2DComponent, Restitution))
            .property("float RestitutionThreshold", offsetof(BoxCollider2DComponent, RestitutionThreshold));

        asbind20::ref_class<ParticleEmmiterComponent>(
            engine,
            "ParticleEmmiterComponent",
            asOBJ_NOCOUNT
        )
            .property("Float4 MinColor", offsetof(ParticleEmmiterComponent, MinColor))
            .property("Float4 MaxColor", offsetof(ParticleEmmiterComponent, MaxColor))
            .property("Float3 MinOffset", offsetof(ParticleEmmiterComponent, MinOffset))
            .property("Float3 MaxOffset", offsetof(ParticleEmmiterComponent, MaxOffset))
            .property("Float2 MinVelocity", offsetof(ParticleEmmiterComponent, MinVelocity))
            .property("Float2 MaxVelocity", offsetof(ParticleEmmiterComponent, MaxVelocity))
            .property("Float2 MinAccel", offsetof(ParticleEmmiterComponent, MinAccel))
            .property("Float2 MaxAccel", offsetof(ParticleEmmiterComponent, MaxAccel))
            .property("float MinLife", offsetof(ParticleEmmiterComponent, MinLife))
            .property("float MaxLife", offsetof(ParticleEmmiterComponent, MaxLife))
            .property("float SpawnInterval", offsetof(ParticleEmmiterComponent, SpawnInterval))
            .property("float SpawnTimer", offsetof(ParticleEmmiterComponent, SpawnTimer));
    }


    template<typename T>
    static bool HasComponentProxy(GameObject* this_) {
        return this_->HasComponent<T>();
    }

    template<typename T>
    static T& AddComponentProxy(GameObject* this_) {
        return this_->AddComponent<T>();
    }

    template<typename T>
    static void RemoveComponentProxy(GameObject* this_) {
        this_->RemoveComponent<T>();
    }

    template<typename T>
    static T& GetComponentProxy(GameObject* this_) {
        return this_->GetComponent<T>();
    }

    template<typename T>
    static const T& ConstGetComponentProxy(GameObject* this_) {
        return this_->GetComponent<T>();
    }

    #define REGISTER_COMPONENT_METHODS(C) \
    .method("bool Has"#C"()", &HasComponentProxy<C>) \
    .method(#C"@ Add"#C"()", &AddComponentProxy<C>) \
    .method(#C"@ Get"#C"()", &GetComponentProxy<C>) \
    .method("void Remove"#C"()", &RemoveComponentProxy<C>)

    #define REGISTER_CONST_COMPONENT_METHODS(C) \
        .method("bool Has"#C"()", &HasComponentProxy<C>) \
        .method("const "#C"@ Get"#C"()", &ConstGetComponentProxy<C>) \

    static void BindObjectAndScene(asIScriptEngine* engine) {
        auto& gameObjectClass = asbind20::value_class<GameObject>(
            engine,
            "GameObject",
            asOBJ_APP_CLASS_ALLINTS
        )
            .behaviours_by_traits()
            .opEquals()
            .opImplConv<bool>()
            REGISTER_CONST_COMPONENT_METHODS(IDComponent)
            REGISTER_CONST_COMPONENT_METHODS(TagComponent)
            REGISTER_COMPONENT_METHODS(RigidBody2DComponent)
            REGISTER_COMPONENT_METHODS(BoxCollider2DComponent)
            REGISTER_COMPONENT_METHODS(ScriptComponent)
            REGISTER_COMPONENT_METHODS(Transform2DComponent)
            REGISTER_COMPONENT_METHODS(Sprite2DComponent)
            REGISTER_COMPONENT_METHODS(Camera2DComponent)
            REGISTER_COMPONENT_METHODS(ParticleEmmiterComponent);

        asbind20::ref_class<Scene>(engine, "Scene", asOBJ_NOCOUNT)
            .method("void Reset()", &Scene::Reset)
            .method("void Start()", &Scene::Start)
            .method("void Stop()", &Scene::Stop)
            .method("const string& GetName() const", &Scene::GetName)
            .method("bool IsObjectValid(GameObject object)", &Scene::IsObjectValid)
            .method("GameObject CreateGameObject(const string& in = \"\")", &Scene::CreateGameObject)
            .method("GameObject CreateGameObjectUUID(UUID uuid, const string& in = \"\")", &Scene::CreateGameObjectUUID)
            .method("GameObject GetGameObject(const string& in)", &Scene::GetObjectByName)
            .method("GameObject DuplicateObject(GameObject object)", &Scene::DuplicateObject)
            .method("void DeleteGameObject(GameObject object)", &Scene::DeleteGameObject);
    }

    namespace Utils {
        void BindGameObjects(asIScriptEngine* engine) {
            BindComponents(engine);
            BindObjectAndScene(engine);
        }
    }
}