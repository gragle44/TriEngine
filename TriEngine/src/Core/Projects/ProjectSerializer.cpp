#include "tripch.h"
#include "ProjectSerializer.h"
#include "Core/Base/Assert.h"

#define KEY(x) << YAML::Key << x

#define VAL(x) << YAML::Value << x

#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap

#define MAP_END   << YAML::EndMap

static constexpr const char* projectHeader = "TriEngine Project File";

namespace TriEngine {
	ProjectSerializer::ProjectSerializer(Reference<Project> project)
		:m_Project(project) {}

	void ProjectSerializer::Serialize(const std::filesystem::path& filePath)
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
		out << YAML::EndMap;

		TRI_CORE_ASSERT(out.good(), "Failed to serialize project");

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void ProjectSerializer::Deserialize(const std::filesystem::path& filePath)
	{
		auto project = YAML::LoadAllFromFile(filePath.string());

		std::string header;
		if (project[0].IsScalar())
			header = project[0].as<std::string>();

		if (header != projectHeader) {
			TRI_CORE_ERROR("Deserialized file was not of type TriEngine Project: \"{0}\"", filePath);
			return;
		}

		ProjectData& data = m_Project->GetProjectData();

		data.Name = project[1]["Project"].as<std::string>();
		data.StartupSceneID = project[1]["StartupSceneID"].as<uint64_t>();

		data.WindowSettings.Width = project[1]["WindowSettings"]["Width"].as<uint32_t>();
		data.WindowSettings.Height = project[1]["WindowSettings"]["Height"].as<uint32_t>();
		data.WindowSettings.Resizable = project[1]["WindowSettings"]["Resizable"].as<bool>();
	}
}
