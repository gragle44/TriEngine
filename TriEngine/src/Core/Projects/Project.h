#pragma once

#include <string>
#include <filesystem>

#include "Core/Resources/Resource.h"

namespace TriEngine {
	struct ProjectData {
		std::string Name = "Untitled Project";
		ResourceID StartupSceneID = 0;
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
