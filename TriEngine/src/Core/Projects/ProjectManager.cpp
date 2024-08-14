#include "tripch.h"

#include "ProjectManager.h"
#include "ProjectSerializer.h"

namespace TriEngine {
	Reference<Project> ProjectManager::CreateNew()
	{
		s_LoadedProject = std::make_shared<Project>();
		return s_LoadedProject;
	}

	Reference<Project> ProjectManager::Load(std::filesystem::path& path)
	{
		Reference<Project> newProject = std::make_shared<Project>();

		ProjectSerializer s(newProject);
		s.Deserialize(path);
		newProject->m_WorkingDirectory = path.parent_path();

		s_LoadedProject = newProject;
		return s_LoadedProject;
	}

	void ProjectManager::Save(std::string_view path)
	{
		ProjectSerializer s(s_LoadedProject);
		s.Serialize(path.data());
	}
}