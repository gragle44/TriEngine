#pragma once

#include "Core/Base/Core.h"
#include "entt/entt.hpp"

namespace YAML {
	class Emitter;
	class Node;
}

namespace TriEngine {
	class Scene;
    class GameObject;

    class SceneSerializer {
	public:
		SceneSerializer(const Reference<Scene>& scene);

		void Serialize(const std::string &filePath);
		void Serialize(std::ostream& stream);
		void Deserialize(const std::string &filePath);
		void Deserialize(std::istream& stream);
	private:
		void SerializeEntity(YAML::Emitter& out, GameObject entity);
		void DeserializeEntity(YAML::Node& serializedEntity);

		Reference<Scene> m_Scene;
	};

	class EntitySerializer {
	public:
		static void Serialize(YAML::Emitter& out, entt::registry& registry, entt::entity entity);
		static entt::entity Deserialize(YAML::Node& serializedEntity, entt::registry& registry);
	};
}
