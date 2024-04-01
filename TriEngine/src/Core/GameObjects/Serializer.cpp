#pragma once
#include "tripch.h"

#include "Serializer.h"
#include "GameObject.h"
#include "Components.h"
#include "Script.h"

#include <glm/glm.hpp>

#include <fstream>

#define KEY(x) << YAML::Key << x

#define VAL(x) << YAML::Value << x

#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap

#define MAP_END   << YAML::EndMap

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

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
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

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
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

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
			rhs.w = node[3].as<int>();
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

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, GameObject& object)
	{
		out << YAML::BeginMap << YAML::Key << "Entity" << YAML::Value << "uuid";

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

			out << YAML::EndMap;
		}

		if (object.HasComponent<Camera2DComponent>()) {
			auto& component = object.GetComponent<Camera2DComponent>();
			out << YAML::Key << "Camera2DComponent" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Zoom" << YAML::Value << component.Camera.m_Zoom;
			out << YAML::Key << "AspectRatio" << YAML::Value << component.Camera.m_AspectRaio;
			out << YAML::Key << "YScale" << YAML::Value << component.Camera.m_YScale;
			out << YAML::Key << "NearClip" << YAML::Value << component.Camera.m_NearClip;
			out << YAML::Key << "FarClip" << YAML::Value << component.Camera.m_FarClip;
			out << YAML::Key << "Resizeable" << YAML::Value << component.Resizeable;

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

	SceneSerializer::SceneSerializer(Scene* scene)
		:m_Scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << "TriEngine Scene File";
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;

		for (auto entity : m_Scene->m_Registry.view<entt::entity>()) {
			GameObject object(entity, m_Scene);
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

	}
	
}
