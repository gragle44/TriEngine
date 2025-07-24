#pragma once

#include "Resource.h"

#include "ResourceLoader.h"
#include "ResourceRegistry.h"

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
		static void InitBinary();
		static void Init();
		static void Shutdown();

		static bool ResourceExists(ResourceID id) { return s_ResourceRegistry->ResourceExists(id); }
		static bool ResourceLoaded(ResourceID id) { return s_Resources.contains(id); }
		
		static Reference<Resource> Load(const ResourceMetadata& metadata);
		static void Remove(ResourceID id);

		static void SaveResource(Reference<Resource> resource);

		static const ResourceRegistry* GetResourceRegistry() { return s_ResourceRegistry; }

		static ResourceID GetIDFromPath(const std::string& path);
		static Reference<Resource> Get(ResourceID id);
		static ResourceMetadata& GetMetadata(ResourceID id);

		static void CreateResourceArchive();

		template <typename T, typename... Args>
		static Reference<T> Create(const std::string& filePath, Args &&...args) {
			static_assert(std::is_base_of<Resource, T>::value, "Can only create resources that extend Resource");

			ResourceMetadata metadata;
			metadata.ID = UUID();
			metadata.Type = GetTypeFromExtension(filePath);
			metadata.Filepath = filePath;

			s_ResourceRegistry->SetMetaData(metadata.ID, metadata);
			s_ResourceRegistry->Save();

			Reference<T> resource = std::reinterpret_pointer_cast<T>(Load(metadata));

			s_Resources[metadata.ID] = resource;
			SaveResource(resource);

			return resource;
		}

	private:
		static std::unordered_map<ResourceID, Reference<Resource>> s_Resources;
		static ResourceRegistry* s_ResourceRegistry;

		static void FreeUnused();

		static ResourceType GetTypeFromExtension(const std::filesystem::path& filePath);

		static ResourceType GetTypeFromString(const std::string& type);
	};
}
