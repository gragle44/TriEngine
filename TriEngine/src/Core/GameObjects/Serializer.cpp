#pragma once
#include "tripch.h"

#include "Serializer.h"
#include "GameObject.h"
#include "Components.h"
#include "Script.h"
#include "ScriptRegistry.h"

#include <glm/glm.hpp>

#include <fstream>

#define KEY(x) << YAML::Key << x

#define VAL(x) << YAML::Value << x

#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap

#define MAP_END   << YAML::EndMap


#define SCENE_HEADER_TEXT "TriEngine Scene File"

namespace YAML {
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}


namespace TriEngine {
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	static std::string RigidBodyTypeToString(RigidBody2DComponent::BodyType type) {
		switch (type)
		{
		case TriEngine::RigidBody2DComponent::BodyType::Static: return "Static";
		case TriEngine::RigidBody2DComponent::BodyType::Dynamic: return "Dynamic";
		case TriEngine::RigidBody2DComponent::BodyType::Kinematic: return "Kinematic";
		default: TRI_CORE_ERROR("Unkown RigidBody type!"); return "";
		}
	}

	static RigidBody2DComponent::BodyType StringToRigidBodyType(const std::string& type) {
		if (type == "Static") return RigidBody2DComponent::BodyType::Static;
		else if (type == "Dynamic") return RigidBody2DComponent::BodyType::Dynamic;
		else if (type == "Kinematic") return RigidBody2DComponent::BodyType::Kinematic;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, GameObject& object)
	{
		auto& idComponent = object.GetComponent<IDComponent>();

		out << YAML::BeginMap << YAML::Key << "Entity" << YAML::Value << idComponent.ID;

		if (object.HasComponent<TagComponent>()) {
			auto& component = object.GetComponent<TagComponent>();
			out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Tag" << YAML::Value << component.Tag;

			out << YAML::EndMap;
		}

		if (object.HasComponent<Transform2DComponent>()) {
			auto& component = object.GetComponent<Transform2DComponent>();
			out << YAML::Key << "Transform2DComponent" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << component.Position;
			out << YAML::Key << "Rotation" << YAML::Value << component.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << component.Scale;

			out << YAML::EndMap;
		}

		if (object.HasComponent<ScriptComponent>()) {
			auto& component = object.GetComponent<ScriptComponent>();
			out << YAML::Key << "ScriptComponent" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "ScriptActive" << YAML::Value << component.ScriptActive;

			out << YAML::Key << "ScriptName" << YAML::Value << component.ScriptName;

			out << YAML::EndMap;
		}

		if (object.HasComponent<RigidBody2DComponent>()) {
			auto& component = object.GetComponent<RigidBody2DComponent>();

			out << YAML::Key << "RigidBody2DComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Type" << YAML::Value << RigidBodyTypeToString(component.Type);
			out << YAML::EndMap;
		}

		if (object.HasComponent<BoxCollider2DComponent>()) {
			auto& component = object.GetComponent<BoxCollider2DComponent>();

			out << YAML::Key << "BoxCollider2DComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Size" << YAML::Value << component.Size;
			out << YAML::Key << "Density" << YAML::Value << component.Density;
			out << YAML::Key << "Friction" << YAML::Value << component.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << component.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << component.RestitutionThreshold;
			out << YAML::EndMap;
		}

		if (object.HasComponent<Camera2DComponent>()) {
			auto& component = object.GetComponent<Camera2DComponent>();

			out << YAML::Key << "Camera2DComponent" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "Zoom" << YAML::Value << component.Camera.m_Zoom;
			out << YAML::Key << "AspectRatio" << YAML::Value << component.Camera.m_AspectRatio;
			out << YAML::Key << "YScale" << YAML::Value << component.Camera.m_YScale;
			out << YAML::Key << "NearClip" << YAML::Value << component.Camera.m_NearClip;
			out << YAML::Key << "FarClip" << YAML::Value << component.Camera.m_FarClip;
			out << YAML::Key << "Resizeable" << YAML::Value << component.Resizeable;
			out << YAML::Key << "Primary" << YAML::Value << component.Primary;

			out << YAML::EndMap;
		}

		if (object.HasComponent<Sprite2DComponent>()) {
			auto& component = object.GetComponent<Sprite2DComponent>();
			out << YAML::Key << "Sprite2DComponent" << YAML::Value << YAML::BeginMap;
			
			// TODO: save other parameters from the texture
			out << YAML::Key << "Texture" << YAML::Value << YAML::BeginMap;
			out << YAML::Key << "FilePath" << YAML::Value << component.Texture->GetFilePath();
			out << YAML::EndMap;

			out << YAML::Key << "Tint" << YAML::Value << component.Tint;
			out << YAML::Key << "TilingFactor" << YAML::Value << component.TilingFactor;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::DeserializeEntity(YAML::Node& entity)
	{
		uint64_t uuid = entity["Entity"].as<uint64_t>();

		std::string name;
		if (entity["TagComponent"] and entity["TagComponent"]["Tag"])
			name = entity["TagComponent"]["Tag"].as<std::string>();

		GameObject newEntity = m_Scene->CreateGameObjectUUID(uuid, name);

		if (entity["Transform2DComponent"]) {
			auto& transform = newEntity.AddComponent<Transform2DComponent>();
			transform.Position = entity["Transform2DComponent"]["Position"].as<glm::vec3>();
			transform.Rotation = entity["Transform2DComponent"]["Rotation"].as<float>();
			transform.Scale = entity["Transform2DComponent"]["Scale"].as<glm::vec2>();
		}

		if (entity["ScriptComponent"]) {
			std::string scriptName = entity["ScriptComponent"]["ScriptName"].as<std::string>();
			if (ScriptRegistry::Exists(scriptName)) {
				auto& script = newEntity.AddComponent<ScriptComponent>();
				script.ScriptActive = entity["ScriptComponent"]["ScriptActive"].as<bool>();
				script.Bind(scriptName);
			}
			else {
				TRI_CORE_WARN("Could not find script \"{0}\" in registry, removing ScriptComponent from entity {1}" ,scriptName, name);
			}
		}

		if (entity["RigidBody2DComponent"]) {
			auto& rigidBody = newEntity.AddComponent<RigidBody2DComponent>();
			rigidBody.Type = StringToRigidBodyType(entity["RigidBody2DComponent"]["Type"].as<std::string>());
		}

		if (entity["BoxCollider2DComponent"]) {
			auto& transform = newEntity.AddComponent<BoxCollider2DComponent>();
			transform.Size = entity["BoxCollider2DComponent"]["Size"].as<glm::vec2>();
			transform.Density = entity["BoxCollider2DComponent"]["Density"].as<float>();
			transform.Friction = entity["BoxCollider2DComponent"]["Friction"].as<float>();
			transform.Restitution = entity["BoxCollider2DComponent"]["Restitution"].as<float>();
			transform.RestitutionThreshold = entity["BoxCollider2DComponent"]["RestitutionThreshold"].as<float>();
		}

		if (entity["Camera2DComponent"]) {
			auto& camera = newEntity.AddComponent<Camera2DComponent>();
			camera.Camera.m_Zoom = entity["Camera2DComponent"]["Zoom"].as<float>();
			camera.Camera.m_AspectRatio = entity["Camera2DComponent"]["AspectRatio"].as<float>();
			camera.Camera.m_YScale = entity["Camera2DComponent"]["YScale"].as<float>();
			camera.Camera.m_NearClip = entity["Camera2DComponent"]["NearClip"].as<float>();
			camera.Camera.m_FarClip = entity["Camera2DComponent"]["FarClip"].as<float>();
			camera.Resizeable = entity["Camera2DComponent"]["Resizeable"].as<bool>();
			camera.Primary = entity["Camera2DComponent"]["Primary"].as<bool>();
		}

		if (entity["Sprite2DComponent"]) {
			auto& sprite = newEntity.AddComponent<Sprite2DComponent>();
			//TODO: different types of sprite components under a dropdown in the UI
			std::string path = entity["Sprite2DComponent"]["Texture"]["FilePath"].as<std::string>();
			if (!path.empty())
				sprite.Texture = Texture2D::Create(path);
			else {
				sprite.Texture = Texture2D::Create(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
			sprite.Tint = entity["Sprite2DComponent"]["Tint"].as<glm::vec4>();
			sprite.TilingFactor = entity["Sprite2DComponent"]["TilingFactor"].as<float>();
		}

	}

	SceneSerializer::SceneSerializer(const Reference<Scene>& scene)
		:m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << SCENE_HEADER_TEXT;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;

		for (auto entity : m_Scene->m_Registry.view<entt::entity>()) {
			GameObject object(entity, m_Scene.get());
			SerializeEntity(out, object);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;
		
		TRI_CORE_ASSERT(out.good(), "Failed to serialize scene");

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void SceneSerializer::Deserialize(const std::string& filePath)
	{
		auto scene = YAML::LoadAllFromFile(filePath);

		if (!scene[0].IsDefined()) {
			TRI_CORE_ERROR("Deserialized file was invalid: \"{0}\"", filePath);
			return;
		}

		std::string header; 
		if (scene[0].IsScalar())
			header = scene[0].as<std::string>();

		if (header != SCENE_HEADER_TEXT) {
			TRI_CORE_ERROR("Deserialized file was not of type TriEngine Scene: \"{0}\"", filePath);
			return;
		}

		std::string name;
		name = scene[1]["Scene"].as<std::string>();

		m_Scene->m_Name = name;
		m_Scene->m_Registry.clear();

		auto entities = scene[1]["Objects"];

		for (auto entity : entities) {
			DeserializeEntity(entity);
		}
	}
	
}
