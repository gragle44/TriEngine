#pragma once

#include <string>
#include <filesystem>

#include "Core/Resources/Resource.h"

namespace TriEngine {

	struct ProjectWindowSettings {
		uint32_t Width = 1280;
		uint32_t Height = 720;
		bool Resizable = true;
	};

	struct ProjectData {
		std::string Name = "Untitled Project";
		ResourceID StartupSceneID = 0;

		ProjectWindowSettings WindowSettings;
	};

	class Project {
	public:
		Project();
		Project(ProjectData& data);

		ProjectData& GetProjectData() { return m_Data; }
		const std::filesystem::path& GetWorkingDirectory() const { return m_ProjectDirectory; }
		const std::filesystem::path GetAbsolutePath(const std::string& relativePath) const { return m_ProjectDirectory / relativePath; }

	private:
		std::filesystem::path m_ProjectDirectory;
		ProjectData m_Data;

		friend class ProjectManager;
	};

}
