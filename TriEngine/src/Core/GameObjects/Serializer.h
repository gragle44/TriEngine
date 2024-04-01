#pragma once

#include "Core/Base/Core.h"
#include "Scene.h"
#include "entt/entt.hpp"

#include "yaml-cpp/yaml.h"

namespace TriEngine {
	class SceneSerializer {
	public:
		SceneSerializer(Scene* scene);

		void Serialize(const std::string& filePath);
		void Deserialize(const std::string& filePath);
	private:
		Scene* m_Scene;
	};
}
