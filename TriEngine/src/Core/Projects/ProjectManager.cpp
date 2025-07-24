#include "tripch.h"

#include "ProjectManager.h"
#include "ProjectSerializer.h"

namespace TriEngine {
	Reference<Project> ProjectManager::CreateNew(const std::filesystem::path& path)
	{
		s_LoadedProject = std::make_shared<Project>();
		s_LoadedProject->m_ProjectDirectory = path.parent_path();
		return s_LoadedProject;
	}

	Reference<Project> ProjectManager::Load(const std::filesystem::path& path, bool binary)
	{
		TRI_CORE_ASSERT(std::filesystem::exists(path), "Invalid file path");
		Reference<Project> newProject = std::make_shared<Project>();

		ProjectSerializer s(newProject);
		if (binary) {
			std::ifstream file(path, std::ios::binary);
			s.Deserialize(file);
		}
		else {
			std::ifstream file(path);
			s.Deserialize(file);
		}

		newProject->m_ProjectDirectory = path.parent_path();

		s_LoadedProject = newProject;
		s_LoadedProject->m_Data.Binary = binary;
		return s_LoadedProject;
	}

	void ProjectManager::Save(std::string_view path, bool binary)
	{
		ProjectSerializer s(s_LoadedProject);
		if (binary) {
			std::ofstream file(path.data(), std::ios::binary);
			s.Serialize(file);
		}
		else {
			s.Serialize(path.data());
		}
	}
}