#pragma once

#include "Resource.h"

#include "ResourceLoader.h"

#include <filesystem>
#include <unordered_map>

namespace TriEngine {

	namespace Utils {
		inline std::string ExtensionFromResourceType(ResourceType type) {
			switch (type)
			{
			case TriEngine::ResourceType::Texture: return "png";
			case TriEngine::ResourceType::Scene: return "tscn";
			default: return "";
			}
		}
	}

	class ResourceManager {

	public:
		static void Init();
		static void Shutdown();

		static bool ResourceExists(ResourceID id) { return s_ResourceRegistry.contains(id); }
		static bool ResourceLoaded(ResourceID id) { return s_Resources.contains(id); }
		
		static Reference<Resource> Load(ResourceMetadata& metadata);
		static void Remove(ResourceID id);

		static void SaveResource(Reference<Resource> resource);

		static const std::unordered_map<ResourceID, ResourceMetadata>& GetResourceRegistry() { return s_ResourceRegistry; }

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

			s_ResourceRegistry[metadata.ID] = metadata;
			SaveResourceRegistry();

			Reference<T> resource = std::reinterpret_pointer_cast<T>(Load(metadata));

			s_Resources[metadata.ID] = resource;
			SaveResource(resource);

			return resource;
		}

		static void LoadResourceRegistry();
		static void SaveResourceRegistry();

	private:
		static std::unordered_map<ResourceID, Reference<Resource>> s_Resources;
		static std::unordered_map<ResourceID, ResourceMetadata> s_ResourceRegistry;

		static std::filesystem::path s_ResourceRegistryPath;

		static ResourceType GetTypeFromExtension(const std::filesystem::path& filePath);

		static std::string GetStringFromType(ResourceType type);
		static ResourceType GetTypeFromString(const std::string& type);
	};
}
