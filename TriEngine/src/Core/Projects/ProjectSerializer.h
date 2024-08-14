#pragma once

#include "Core/Base/Core.h"
#include "Project.h"

#include "yaml-cpp/yaml.h"

namespace TriEngine {
	class ProjectSerializer {
	public:
		ProjectSerializer(Reference<Project> project);

		void Serialize(const std::filesystem::path& filePath);
		void Deserialize(const std::filesystem::path& filePath);
	private:
		Reference<Project> m_Project;
	};
}