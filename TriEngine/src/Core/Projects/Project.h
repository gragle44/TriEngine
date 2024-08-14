#pragma once

#include <string>
#include <filesystem>

namespace TriEngine {
	struct ProjectData {
		std::string Name = "Untitled Project";
		std::filesystem::path StartupScene;
	};

	class Project {
	public:
		Project();
		Project(ProjectData& data);

		ProjectData& GetProjectData() { return m_Data; }
		const std::filesystem::path& GetWorkingDirectory() const { return m_WorkingDirectory; }
		const std::filesystem::path GetAbsolutePath(const std::string& relativePath) const { return m_WorkingDirectory / relativePath; }

	private:
		std::filesystem::path m_WorkingDirectory;
		ProjectData m_Data;

		friend class ProjectManager;
	};

}
