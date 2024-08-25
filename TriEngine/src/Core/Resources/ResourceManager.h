#pragma once

#include "Resource.h"

namespace TriEngine {
	class ResourceManager {
	public:
		bool ResourceExists(ResourceID id) { return s_ResourceMetadataCache.contains(id); }
		bool ResourceLoaded(ResourceID id) { return s_Resources.contains(id); }

		Reference<Resource> Load(std::string& path);

		Reference<Resource> Get(ResourceID id);
		ResourceMetadata& GetMetadata(ResourceID id);

	private:
		std::unordered_map<ResourceID, Reference<Resource>> s_Resources;
		std::unordered_map<ResourceID, ResourceMetadata> s_ResourceMetadataCache;

		void UpdateMetadataCache();
	};
}
