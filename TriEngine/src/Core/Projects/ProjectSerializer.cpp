#include "tripch.h"
#include "ProjectSerializer.h"
#include "Core/Base/Assert.h"

#include "Core/Base/Window.h"

#include "magic_enum.hpp"

#define KEY(x) << YAML::Key << x

#define VAL(x) << YAML::Value << x

#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap

#define MAP_END   << YAML::EndMap

static constexpr const char* projectHeader = "TriEngine Project File";

namespace YAML {
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

	Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
}

namespace TriEngine {
	ProjectSerializer::ProjectSerializer(Reference<Project> project)
		:m_Project(project) {}

	void ProjectSerializer::Serialize(const std::filesystem::path& path) {
		std::ofstream file(path);
		Serialize(file);
	}

	void ProjectSerializer::Serialize(std::ostream& stream)
	{
		ProjectData data = m_Project->GetProjectData();

		YAML::Emitter out;

		out << projectHeader;

		out MAP_START;
		out KEYVAL("Project", data.Name);
		out KEYVAL("StartupSceneID", data.StartupSceneID);

		out << YAML::Key << "WindowSettings" << YAML::Value << YAML::BeginMap;
		out KEYVAL("Width", data.WindowSettings.Width);
		out KEYVAL("Height", data.WindowSettings.Height);
		out KEYVAL("Resizable", data.WindowSettings.Resizable);
		out KEYVAL("VSync", magic_enum::enum_name(data.WindowSettings.VSync).data());
		out << YAML::EndMap;

		out << YAML::Key << "RenderingSettings" << YAML::Value << YAML::BeginMap;
		out KEYVAL("ClearColor", data.RenderingSettings.ClearColor);
		out << YAML::EndMap;

		TRI_CORE_ASSERT(out.good(), "Failed to serialize project");

		stream << out.c_str();
	}

	void ProjectSerializer::Deserialize(const std::filesystem::path& path) {
		std::ifstream file(path);
		Deserialize(file);
	}

	void ProjectSerializer::Deserialize(std::istream& stream)
	{
		auto project = YAML::LoadAll(stream);

		std::string header;
		if (project[0].IsScalar())
			header = project[0].as<std::string>();

		if (header != projectHeader) {
			TRI_CORE_ERROR("Deserialized file was not of type TriEngine Project");
			TRI_CORE_ASSERT(false, "See above error");
			return;
		}

		ProjectData& data = m_Project->GetProjectData();

		data.Name = project[1]["Project"].as<std::string>();
		data.StartupSceneID = project[1]["StartupSceneID"].as<uint64_t>();

		data.WindowSettings.Width = project[1]["WindowSettings"]["Width"].as<uint32_t>();
		data.WindowSettings.Height = project[1]["WindowSettings"]["Height"].as<uint32_t>();
		data.WindowSettings.Resizable = project[1]["WindowSettings"]["Resizable"].as<bool>();
		data.WindowSettings.VSync = magic_enum::enum_cast<VsyncMode>(project[1]["WindowSettings"]["VSync"].as<std::string>()).value_or(VsyncMode::On);

		data.RenderingSettings.ClearColor = project[1]["RenderingSettings"]["ClearColor"].as<glm::vec4>();
	}
}
