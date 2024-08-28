#pragma once

#include "Resource.h"

#include "ResourceLoader.h"

#include <filesystem>
#include <unordered_map>

namespace TriEngine {
	class ResourceManager {
	public:
		static void Init();
		static void Shutdown();

		static bool ResourceExists(ResourceID id) { return s_ResourceMetadataCache.contains(id); }
		static bool ResourceLoaded(ResourceID id) { return s_Resources.contains(id); }
		
		static Reference<Resource> Load(ResourceMetadata& metadata);
		static void Remove(ResourceID id);

		static void SaveResourceMetadata(Reference<Resource> resource);

		static ResourceID GetIDFromPath(const std::string& path);
		static Reference<Resource> Get(ResourceID id);
		static ResourceMetadata& GetMetadata(ResourceID id);

		template <typename T, typename... Args>
		static Reference<T> Create(const std::string& filePath, Args &&...args) {
			static_assert(std::is_base_of<Resource, T>::value, "Can only create resources that extend Resource");

			ResourceMetadata metadata;
			metadata.ID = UUID();
			metadata.Type = GetTypeFromExtension(filePath);
			metadata.Filepath = filePath;

			s_ResourceMetadataCache[metadata.ID] = metadata;
			SaveResourceRegistry();

			Reference<T> resource = std::reinterpret_pointer_cast<T>(Load(metadata));

			s_Resources[metadata.ID] = resource;
			SaveResourceMetadata(resource);

			return resource;
		}

	private:
		static std::unordered_map<ResourceID, Reference<Resource>> s_Resources;
		static std::unordered_map<ResourceID, ResourceMetadata> s_ResourceMetadataCache;

		static std::filesystem::path s_ResourceRegistryPath;

		static ResourceType GetTypeFromExtension(const std::filesystem::path& filePath);

		static std::string GetStringFromType(ResourceType type);
		static ResourceType GetTypeFromString(const std::string& type);

		static void LoadResourceRegistry();
		static void SaveResourceRegistry();
	};
}
