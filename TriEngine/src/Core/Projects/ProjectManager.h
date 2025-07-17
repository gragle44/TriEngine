#pragma once

#include "ProjectSerializer.h"
#include "Project.h"
#include <string>
#include <filesystem>

namespace TriEngine {
	class ProjectManager {
	public:
		static Reference<Project> CreateNew(const std::filesystem::path& path);
		static Reference<Project> Load(const std::filesystem::path& path);
		static void Save(std::string_view path);

		static Reference<Project> GetCurrent() { return s_LoadedProject; }

		static bool IsProjectLoaded() { return s_LoadedProject != nullptr; }

		static ProjectData& GetCurrentProjectData() { return s_LoadedProject->GetProjectData(); }

	private:
		inline static Reference<Project> s_LoadedProject = nullptr;
	};
}
