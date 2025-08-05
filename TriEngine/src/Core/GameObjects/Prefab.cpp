#include "tripch.h"

#include "Prefab.h"

#include "Scene.h"
#include "SceneSerializer.h"
#include "GameObject.h"
#include "Components.h"
#include "yaml-cpp/yaml.h"

namespace TriEngine {

    void Prefab::CopyFromObject(entt::entity entity, GameObject object) {
        ++m_EntityCount;
        CopyAllComponents(entity, object);

        m_Registry.emplace<IDComponent>(entity, UUID());

        const auto& relationship = object.GetComponent<RelationshipComponent>();

        auto& newRelationship = m_Registry.emplace<RelationshipComponent>(entity, relationship.Parent);
        newRelationship.Parent = object.GetParent().GetID();

        for (UUID childid : relationship.Children) {
            entt::entity child = m_Registry.create();
            CopyFromObject(child, object.GetScene()->GetObjectByID(childid));
            newRelationship.Children.push_back(m_Registry.get<IDComponent>(child).ID);
        }
    }

    Prefab::Prefab(GameObject object)
        : m_Registry() {
        MetaData.Type = ResourceType::Prefab;
        TRI_CORE_ASSERT(object, "Tried to create prefab from an invalid object");
        TRI_CORE_ASSERT(!object.HasParent(), "Only root objects can be into made prefabs");

        entt::entity prefab = m_Registry.create();
        m_Root = prefab;
        CopyFromObject(prefab, object);
    }

    static constexpr const char* PREFAB_HEADER = "TriEngine Prefab File";

    Prefab::Prefab(std::istream& data)
        :m_Registry() {
        MetaData.Type = ResourceType::Prefab;

        auto prefab = YAML::LoadAll(data);

        if (!prefab[0].IsDefined()) {
            TRI_CORE_ERROR("Deserialized prefab data stream was invalid");
            return;
        }

        std::string header;
        if (prefab[0].IsScalar())
            header = prefab[0].as<std::string>();

        if (header != PREFAB_HEADER) {
            TRI_CORE_ERROR("Deserialized file was not of type TriEngine Prefab");
            return;
        }

        auto entities = prefab[1]["Objects"];

        for (auto entity : entities) {
            EntitySerializer::Deserialize(entity, m_Registry);
            ++m_EntityCount;
        }

        auto view = m_Registry.view<RelationshipComponent>();

        for (auto entity : view) {
            auto& relationship = view.get<RelationshipComponent>(entity);
            if (relationship.Parent == static_cast<UUID>(0)) {
                m_Root = entity;
                break;
            }
        }
    }

    void Prefab::Save(std::ostream& outFile) {

        YAML::Emitter out;
        out << PREFAB_HEADER;
        out << YAML::BeginMap;
        out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;

        for (auto entity : m_Registry.view<entt::entity>()) {
            EntitySerializer::Serialize(out, m_Registry, entity);
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;

        TRI_CORE_ASSERT(out.good(), "Failed to serialize prefab");

        outFile.write(out.c_str(), out.size());
    }

    void Prefab::CopyAllComponents(entt::entity entity, GameObject object) {
        CopyComponentFromGameObject<PrefabComponent>(entity, object);
        CopyComponentFromGameObject<TagComponent>(entity, object);
        CopyComponentFromGameObject<Transform2DComponent>(entity, object);
        CopyComponentFromGameObject<TransformComponent>(entity, object);
        CopyComponentFromGameObject<RigidBody2DComponent>(entity, object);
        CopyComponentFromGameObject<BoxCollider2DComponent>(entity, object);
        CopyComponentFromGameObject<NativeScriptComponent>(entity, object);
        CopyComponentFromGameObject<ScriptComponent>(entity, object);
        CopyComponentFromGameObject<Camera2DComponent>(entity, object);
        CopyComponentFromGameObject<Sprite2DComponent>(entity, object);
        CopyComponentFromGameObject<ParticleEmmiterComponent>(entity, object);
    }
}