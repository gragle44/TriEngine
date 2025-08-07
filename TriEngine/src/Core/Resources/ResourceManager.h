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
			case TriEngine::ResourceType::Script: return "as";
			case TriEngine::ResourceType::Prefab: return "prefab";
			default: return "";
			}
		}
	}

	class ResourceManager {
	public:
		static void Init();
		static void Shutdown();

		static bool ResourceExists(ResourceID id) { return s_ResourceRegistry->ResourceExists(id); }
		static bool ResourceLoaded(ResourceID id) { return s_Resources.contains(id); }
		
		static Reference<Resource> Load(const ResourceMetadata& metadata);
		static void Remove(ResourceID id);

		static void SaveResource(Reference<Resource> resource);

		static const ResourceRegistry* GetResourceRegistry() { return s_ResourceRegistry; }

		static std::string GetRelativePath(std::string_view absolutePath);
		static std::string GetAbsolutePath(std::string_view relativePath);

		static ResourceID GetIDFromPath(std::string_view relativePath);
        static Reference<Resource> Get(ResourceID id);
        static Reference<Resource> Get(const std::string& relativePath) { return Get(GetIDFromPath(relativePath)); }
        static ResourceMetadata& GetMetadata(ResourceID id);

		static void CreateResourceArchive();

		template <typename T, typename... Args>
		static Reference<T> Create(const std::string& filePath, Args &&...args) {
			static_assert(std::is_base_of<Resource, T>::value, "Can only create resources that extend Resource");

			ResourceMetadata metadata;
			metadata.ID = UUID();
			metadata.Type = GetTypeFromExtension(filePath);
			metadata.Filepath = GetRelativePath(filePath);

			s_ResourceRegistry->SetMetaData(metadata.ID, metadata);
			s_ResourceRegistry->Save();

			Reference<T> resource = std::reinterpret_pointer_cast<T>(Load(metadata));

			s_Resources[metadata.ID] = resource;
			SaveResource(resource);

			return resource;
		}

        template <typename T>
        static void Create(Reference<T> resource, const std::string& filePath) {
            static_assert(std::is_base_of<Resource, T>::value, "Can only create resources that extend Resource");

			auto& metadata = resource->MetaData;

            metadata.ID = UUID();
			auto type = metadata.Type = GetTypeFromExtension(filePath);
			TRI_CORE_ASSERT(metadata.Type == type, "Type from filepath and resource type do not match");
            metadata.Type = type;
            metadata.Filepath = GetRelativePath(filePath);

            s_ResourceRegistry->SetMetaData(metadata.ID, metadata);
            s_ResourceRegistry->Save();

            s_Resources[metadata.ID] = resource;
            SaveResource(resource);
        }

    private:
		static std::unordered_map<ResourceID, Reference<Resource>> s_Resources;
		static ResourceRegistry* s_ResourceRegistry;

		static void FreeUnused();

		static ResourceType GetTypeFromExtension(const std::filesystem::path& filePath);

		static ResourceType GetTypeFromString(const std::string& type);
	};
}
