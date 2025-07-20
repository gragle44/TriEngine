#include "tripch.h"

#include "ResourceManager.h"
#include "Projects/ProjectManager.h"

#include "magic_enum.hpp"

namespace TriEngine {

	std::unordered_map<ResourceID, Reference<Resource>> ResourceManager::s_Resources;
	std::unordered_map<ResourceID, ResourceMetadata> ResourceManager::s_ResourceRegistry;

	std::filesystem::path ResourceManager::s_ResourceRegistryPath;

	void ResourceManager::Init()
	{
		s_ResourceRegistryPath = ProjectManager::GetCurrent()->GetWorkingDirectory() / "resources.trireg";
		LoadResourceRegistry();
	}

	void ResourceManager::Shutdown()
	{
		s_Resources.clear();
		s_ResourceRegistry.clear();
	}

	void ResourceManager::ReloadResource(ResourceID resourceID) noexcept {
		if (s_Resources.contains(resourceID))
			ReloadResource(s_Resources.at(resourceID));
		else
			TRI_CORE_ERROR("Cannot reload resource: invalid resource ID: '{}'", resourceID);
	}

	void ResourceManager::ReloadResource(Reference<Resource> resource) noexcept
	{
		TRI_CORE_ASSERT(s_Resources.contains(resource->MetaData.ID), "ResourceManager doesn't contain resource");
		Reference<Resource> newResource = ResourceLoader::Load(resource->MetaData);
		s_Resources[resource->MetaData.ID] = newResource;
		*resource = *newResource;
	}

	Reference<Resource> ResourceManager::Load(ResourceMetadata& metadata)
	{
		Reference<Resource> resource = ResourceLoader::Load(metadata);
		s_Resources[metadata.ID] = resource;
		return resource;
	}

	void ResourceManager::Remove(ResourceID id)
	{
		if (ResourceExists(id)) {
			ResourceMetadata metadata = s_ResourceRegistry[id];

			std::string resourceMetadataPath = metadata.Filepath + ".meta";

			if (std::filesystem::exists(resourceMetadataPath))
				std::filesystem::remove(resourceMetadataPath);

			s_ResourceRegistry.erase(id);
			SaveResourceRegistry();
		}
	}

	void ResourceManager::SaveResource(Reference<Resource> resource)
	{
		ResourceLoader::Save(resource);
	}

	ResourceID ResourceManager::GetIDFromPath(const std::string& path)
	{
		for (const auto& [id, metadata] : s_ResourceRegistry) {
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
		return s_ResourceRegistry.at(id);
	}

	ResourceType ResourceManager::GetTypeFromExtension(const std::filesystem::path& filePath)
	{
		std::filesystem::path extension = filePath.extension();
		return GetTypeFromString(extension.generic_string());
	}

	std::string ResourceManager::GetStringFromType(ResourceType type)
	{
		switch (type)
		{
			case TriEngine::ResourceType::Texture: return "Texture";
			case TriEngine::ResourceType::Scene: return "Scene";
			case TriEngine::ResourceType::Script: return "Script";
			default: return "None";
		}
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

	void ResourceManager::LoadResourceRegistry()
	{

		if (!std::filesystem::exists(s_ResourceRegistryPath)) {
			TRI_CORE_WARN("Couldn't find Resource Registry: {0}", s_ResourceRegistryPath);
			return;
		}

		auto registry = YAML::LoadAllFromFile(s_ResourceRegistryPath.generic_string());

		auto registryMetadata = registry[1];

		for (auto data : registryMetadata) {
			ResourceMetadata metadata;
			metadata.ID = data["ID"].as<uint64_t>();
			metadata.Filepath = ProjectManager::GetCurrent()->GetAbsolutePath(data["Filepath"].as<std::string>()).string();
			metadata.Type = magic_enum::enum_cast<ResourceType>(data["Type"].as<std::string>()).value_or(ResourceType::None);
			s_ResourceRegistry[metadata.ID] = metadata;
		}
	}

	void ResourceManager::SaveResourceRegistry()
	{
		YAML::Emitter out;

		out << "TriEngine Resource Registry";

		out << YAML::BeginSeq;

		for (const auto& [id, metadata] : s_ResourceRegistry) {
			out << YAML::BeginMap;
			out << YAML::Key << "ID" << YAML::Value << id;
			out << YAML::Key << "Filepath" << YAML::Value << std::filesystem::relative(metadata.Filepath, ProjectManager::GetCurrent()->GetWorkingDirectory()).string();
			out << YAML::Key << "Type" << YAML::Value << GetStringFromType(metadata.Type);
			out << YAML::EndMap;
		}
		out << YAML::EndSeq;

		std::ofstream fout(s_ResourceRegistryPath);
		fout << out.c_str();
	}
}