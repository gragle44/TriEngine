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

	Reference<Project> ProjectManager::Load(const std::filesystem::path& path)
	{
		Reference<Project> newProject = std::make_shared<Project>();

		ProjectSerializer s(newProject);
		s.Deserialize(path);
		newProject->m_ProjectDirectory = path.parent_path();

		s_LoadedProject = newProject;
		return s_LoadedProject;
	}

	void ProjectManager::Save(std::string_view path)
	{
		ProjectSerializer s(s_LoadedProject);
		s.Serialize(path.data());
	}
}