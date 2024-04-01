#pragma once
#include "tripch.h"

#include "Serializer.h"
#include "GameObject.h"
#include "Components.h"

#include <glm/glm.hpp>

#include "yaml-cpp/yaml.h"

#include <fstream>

#define KEY(x) << YAML::Key << x

#define VAL(x) << YAML::Value << x

#define KEYVAL(x, y) KEY(x) VAL(y)


#define MAP_START << YAML::BeginMap

#define MAP_END   << YAML::EndMap

namespace YAML {
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
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<int>();
			rhs.y = node[1].as<int>();
			rhs.z = node[2].as<int>();
			return true;
		}
	};
}


namespace TriEngine {
	static void SerializeEntity(YAML::Emitter& out, GameObject& object)
	{
		out << YAML::BeginMap << YAML::Key << "Entity" << YAML::Value << "uuid";

		if (object.HasComponent<TagComponent>()) {
			auto& component = object.GetComponent<TagComponent>();
			out << YAML::Key << "TagComponent" << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Tag" << YAML::Value << component.Tag;

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
		out << YAML::BeginMap;
		out << YAML::Key << "TriScene" << YAML::Value << m_Scene->m_Name;
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;

		for (auto entity : m_Scene->m_Registry.view<entt::entity>()) {
			GameObject object(entity, m_Scene);
			SerializeEntity(out, object);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void SceneSerializer::Deserialize(const std::string& filePath)
	{

	}

	
}
