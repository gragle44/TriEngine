#pragma once

#include <string>
#include <filesystem>

#include "Core/Resources/Resource.h"
#include "Core/Base/Window.h"

#include "glm/vec4.hpp"

namespace TriEngine {
	struct ProjectWindowSettings {
		uint32_t Width = 1280;
		uint32_t Height = 720;
		VsyncMode VSync = VsyncMode::On;
		bool Resizable = true;
	};

	struct ProjectRenderingSettings {
		glm::vec4 ClearColor = { 0.15f, 0.15f, 0.15f, 1.0f };
	};

	struct ProjectData {
		std::string Name = "Untitled Project";
		ResourceID StartupSceneID = 0;
		bool Binary = false;

		ProjectWindowSettings WindowSettings;
		ProjectRenderingSettings RenderingSettings;
	};

	class Project {
	public:
		Project();
		Project(ProjectData& data);

		ProjectData& GetProjectData() { return m_Data; }
		const std::filesystem::path& GetWorkingDirectory() const { return m_ProjectDirectory; }
		const std::filesystem::path GetAbsolutePath(std::string_view relativePath) const { return m_ProjectDirectory / relativePath; }

	private:
		std::filesystem::path m_ProjectDirectory;
		ProjectData m_Data;

		friend class ProjectManager;
	};

}
