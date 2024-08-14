#pragma once

#include "ProjectSerializer.h"
#include "Project.h"
#include <string>
#include <filesystem>

namespace TriEngine {
	class ProjectManager {
	public:
		static Reference<Project> CreateNew();
		static Reference<Project> Load(std::filesystem::path& path);
		static void Save(std::string_view path);

		static Reference<Project> GetCurrent() { return s_LoadedProject; }

	private:
		inline static Reference<Project> s_LoadedProject;
	};
}
