#include "tripch.h"

#include "SceneLoader.h"

#include "GameObjects/Scene.h"
#include "GameObjects/SceneSerializer.h"

namespace TriEngine {
	Reference<Resource> TriEngine::SceneLoader::Load(ResourceMetadata& metadata)
	{
		Reference<Scene> scene;
		TriEngine::SceneSerializer s(scene);
		s.Deserialize(metadata.Filepath);
		scene->MetaData = metadata;
		return scene;
	}

	void SceneLoader::Save(Reference<Resource> resource)
	{
		auto scene = std::dynamic_pointer_cast<Scene>(resource);
		TriEngine::SceneSerializer s(scene);
		s.Serialize(scene->MetaData.Filepath);
	}
}