#include "tripch.h"

#include "ResourceManager.h"

namespace TriEngine {
	Reference<Resource> ResourceManager::Load(std::string& path)
	{
		// Return "None" asset if cant load
		return nullptr;
	}

	Reference<Resource> ResourceManager::Get(ResourceID id)
	{
		if (!ResourceExists(id)) {
			TRI_CORE_WARN("Invalid Resource ID: {0}", id);
			return nullptr;
		}

		if (ResourceLoaded(id))
			return s_Resources.at(id);

		ResourceMetadata& metadata = GetMetadata(id);
		
		Reference<Resource> resource = Load(metadata.Filepath);
		if (!resource) {
			TRI_CORE_ERROR("Failed to load asset: {0}", metadata.Filepath);
		}

		return resource;
	}

	ResourceMetadata& ResourceManager::GetMetadata(ResourceID id)
	{
		return s_ResourceMetadataCache.at(id);
	}
}