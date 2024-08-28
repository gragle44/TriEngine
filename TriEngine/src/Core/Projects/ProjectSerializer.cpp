#include "tripch.h"
#include "ProjectSerializer.h"
#include "Core/Base/Assert.h"

#define KEY(x) << YAML::Key << x

#define VAL(x) << YAML::Value << x

#define KEYVAL(x, y) KEY(x) VAL(y)

#define MAP_START << YAML::BeginMap

#define MAP_END   << YAML::EndMap

#define PROJECT_HEADER_TEXT "TriEngine Project File"

namespace TriEngine {
	ProjectSerializer::ProjectSerializer(Reference<Project> project)
		:m_Project(project) {}

	void ProjectSerializer::Serialize(const std::filesystem::path& filePath)
	{
		ProjectData data = m_Project->GetProjectData();

		YAML::Emitter out;

		out << PROJECT_HEADER_TEXT;

		out MAP_START;
		out KEYVAL("Project", data.Name);
		out KEYVAL("StartupScene", data.StartupScene.string());
		out MAP_END;

		TRI_CORE_ASSERT(out.good(), "Failed to serialize scene");

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	void ProjectSerializer::Deserialize(const std::filesystem::path& filePath)
	{
		auto project = YAML::LoadAllFromFile(filePath.string());


		std::string header;
		if (project[0].IsScalar())
			header = project[0].as<std::string>();

		if (header != PROJECT_HEADER_TEXT) {
			TRI_CORE_ERROR("Deserialized file was not of type TriEngine Project: \"{0}\"", filePath);
			return;
		}

		ProjectData& data = m_Project->GetProjectData();

		data.Name = project[1]["Project"].as<std::string>();
		data.StartupScene = project[1]["StartupScene"].as<std::string>();
	}
}