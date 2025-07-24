#include "tripch.h"

#include "ResourceManager.h"

#include "ResourceArchive.h"

#include "Projects/ProjectManager.h"

#include "magic_enum.hpp"

namespace TriEngine {

	std::unordered_map<ResourceID, Reference<Resource>> ResourceManager::s_Resources;
	ResourceRegistry* ResourceManager::s_ResourceRegistry;

    void ResourceManager::Init()
    {
		auto resourceRegistryPath = ProjectManager::GetCurrent()->GetWorkingDirectory() / "resources.trireg";
		s_ResourceRegistry = ResourceRegistry::Create(resourceRegistryPath);
		s_ResourceRegistry->Load();
	}

	void ResourceManager::Shutdown()
	{
		s_Resources.clear();
		delete s_ResourceRegistry;
	}

	Reference<Resource> ResourceManager::Load(const ResourceMetadata& metadata)
	{
		Reference<Resource> resource;
		if (s_ResourceRegistry->IsBinaryRegistry())
			resource = ResourceLoader::LoadBinary(metadata);
		else
			resource = ResourceLoader::Load(metadata);
		s_Resources[metadata.ID] = resource;
		return resource;
	}

	void ResourceManager::Remove(ResourceID id)
	{
		if (ResourceExists(id)) {
			ResourceMetadata metadata = s_ResourceRegistry->GetMetaData(id);

			std::string resourceMetadataPath = metadata.Filepath + ".meta";

			if (std::filesystem::exists(resourceMetadataPath))
				std::filesystem::remove(resourceMetadataPath);

			s_ResourceRegistry->Remove(id);
			s_ResourceRegistry->Save();
		}
	}

	void ResourceManager::SaveResource(Reference<Resource> resource)
	{
		TRI_CORE_ASSERT(!s_ResourceRegistry->IsBinaryRegistry(), "Resources loaded from binary archives should not be saved during runtime");
		ResourceLoader::Save(resource);
	}

	ResourceID ResourceManager::GetIDFromPath(const std::string& path)
	{
		for (const auto& [id, metadata] : s_ResourceRegistry->GetRegistry()) {
			if (metadata.Filepath == path)
				return metadata.ID;
		}
		return 0;
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
		
		Reference<Resource> resource = Load(metadata);
		if (!resource) {
			TRI_CORE_ERROR("Failed to load asset: {0}", metadata.Filepath);
		}

		return resource;
	}

	ResourceMetadata& ResourceManager::GetMetadata(ResourceID id)
	{
		return s_ResourceRegistry->GetMetaData(id);
	}

    void ResourceManager::CreateResourceArchive()
    {	
		// The pipeline will be:
		// Set resource offset to current offset
		// Save resource
		// Jump to end of file to get new offset

		for (auto& [id, metadata] : s_ResourceRegistry->GetRegistry()) {
			if (!s_Resources.contains(id))
				Load(metadata);
		}

		std::vector<std::vector<Reference<Resource>>> resourcesByIndex;
		// TODO: remove this magic number
		resourcesByIndex.resize(64);

		for (const auto& [id, resource] : s_Resources) {
			resourcesByIndex[resource->MetaData.ArchiveIndex].push_back(resource);
		}

		uint32_t index = 0;
		for (auto& resourceArchiveList : resourcesByIndex) {
			if (resourceArchiveList.empty()) {
				index++;
				continue;
			}

			uint32_t currentOffset = sizeof(ArchiveHeader);

			ArchiveHeader header;
			memcpy(&header.Magic, RESOURCE_HEADER_MAGIC, sizeof(uint32_t));
			header.Version = 0;
			header.Index = index;
			header.FileCount = resourceArchiveList.size();

			std::filesystem::path archivePath = ProjectManager::GetCurrent()->GetAbsolutePath(std::format("export/data/{}.pck", index));
			if (!std::filesystem::exists(archivePath.parent_path()))
				std::filesystem::create_directories(archivePath.parent_path());

			std::ofstream archive(archivePath, std::ios::binary);
			archive.write(reinterpret_cast<char*>(&header), sizeof(header));

			for (auto& resource : resourceArchiveList) {
				resource->MetaData.ArchiveOffset = currentOffset;
				// TODO: pass the stream into savebinary so the file isnt being opened/closed constantly
				ResourceLoader::SaveBinary(resource, archive);
				archive.seekp(0, std::ios::end);
				currentOffset = static_cast<uint32_t>(archive.tellp());
			}

			archive.close();
			index++;
		}

		auto resourceRegistryPath = ProjectManager::GetCurrent()->GetWorkingDirectory() / "export/resources.trireg";
		BinaryResourceRegistry binaryRegistry(resourceRegistryPath);

		// change this to include index and offset once resources are saved earlier
		for (const auto& [id, resource] : s_Resources) {
			binaryRegistry.SetMetaData(id, resource->MetaData);
		}
		binaryRegistry.Save();

		// Free unused resources after creating them earlier
		FreeUnused();
    }

    void ResourceManager::FreeUnused()
    {
		for (auto& [id, resource] : s_Resources) {
			if (resource.use_count() == 1)
				s_Resources.erase(id);
		}
    }

    ResourceType ResourceManager::GetTypeFromExtension(const std::filesystem::path &filePath)
    {
		std::filesystem::path extension = filePath.extension();
		return GetTypeFromString(extension.generic_string());
	}
	
	ResourceType ResourceManager::GetTypeFromString(const std::string& type)
	{
		if (type == ".png") return ResourceType::Texture;
		else if (type == ".jpg") return ResourceType::Texture;
		else if (type == ".jpeg") return ResourceType::Texture;
		else if (type == ".tscn") return ResourceType::Scene;
		else if (type == ".as") return ResourceType::Script;
		else return ResourceType::None;
	}
}