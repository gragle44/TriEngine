#include "tripch.h"

#include "ResourceLoader.h"

#include "TextureLoader.h"
#include "SceneLoader.h"
#include "ScriptLoader.h"

namespace TriEngine {
	using LoadFunction = Reference<Resource>(*)(ResourceMetadata&);
	using SaveFunction = void(*)(Reference<Resource>);

	static std::unordered_map<ResourceType, LoadFunction> s_LoadFunctions = {
		{ResourceType::Texture, TextureLoader::Load},
		{ResourceType::Scene, SceneLoader::Load},
		{ResourceType::Script, ScriptLoader::Load},
	};

	static std::unordered_map<ResourceType, SaveFunction> s_SaveFunctions = {
		{ResourceType::Texture, TextureLoader::Save},
		{ResourceType::Scene, SceneLoader::Save},
		{ResourceType::Script, ScriptLoader::Save},
	};

	Reference<Resource> ResourceLoader::Load(ResourceMetadata& metadata)
	{
		if (metadata.Type == ResourceType::None) {
			TRI_CORE_ERROR("Invalid resource type");
			return nullptr;
		}

		LoadFunction loadfn = s_LoadFunctions[metadata.Type];
		return loadfn(metadata);
	}

	void ResourceLoader::Save(Reference<Resource> resource)
	{
		ResourceMetadata metadata = resource->MetaData;
		if (metadata.Type == ResourceType::None) {
			TRI_CORE_ERROR("Invalid resource type");
			return;
		}

		SaveFunction savefn = s_SaveFunctions[metadata.Type];
		savefn(resource);
	}
}