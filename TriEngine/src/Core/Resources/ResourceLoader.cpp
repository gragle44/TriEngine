#include "tripch.h"

#include "ResourceLoader.h"

#include "TextureLoader.h"

namespace TriEngine {
	using LoadFunction = Reference<Resource>(*)(ResourceMetadata&);
	using SaveFunction = void(*)(Reference<Resource>);

	static std::map<ResourceType, LoadFunction> s_LoadFunctions = {
		{ResourceType::Texture, TextureLoader::Load}
	};

	static std::map<ResourceType, SaveFunction> s_SaveFunctions = {
		{ResourceType::Texture, TextureLoader::Save}
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