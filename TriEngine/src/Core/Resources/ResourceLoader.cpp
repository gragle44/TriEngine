#include "tripch.h"

#include "ResourceLoader.h"

#include "TextureLoader.h"
#include "SceneLoader.h"
#include "ScriptLoader.h"
#include "PrefabLoader.h"

namespace TriEngine {
	using LoadFunction = Reference<Resource>(*)(const ResourceMetadata&);
	using SaveFunction = void (*)(Reference<Resource>);
	using SaveBinaryFunction = void (*)(Reference<Resource>, std::ostream&);

    const static std::unordered_map<ResourceType, LoadFunction> s_LoadFunctions = {
        {ResourceType::Texture, TextureLoader::Load},
        {ResourceType::Scene, SceneLoader::Load},
        {ResourceType::Script, ScriptLoader::Load},
        {ResourceType::Prefab, PrefabLoader::Load}
    };

    const static std::unordered_map<ResourceType, SaveFunction> s_SaveFunctions = {
        {ResourceType::Texture, TextureLoader::Save},
        {ResourceType::Scene, SceneLoader::Save},
        {ResourceType::Script, ScriptLoader::Save},
        {ResourceType::Prefab, PrefabLoader::Save}
    };

    static const std::unordered_map<ResourceType, LoadFunction> s_BinaryLoadFunctions = {
        {ResourceType::Texture, TextureLoader::LoadBinary},
        {ResourceType::Scene, SceneLoader::LoadBinary},
        {ResourceType::Script, ScriptLoader::LoadBinary},
        {ResourceType::Prefab, PrefabLoader::LoadBinary}
    };
    static const std::unordered_map<ResourceType, SaveBinaryFunction> s_BinarySaveFunctions = {
        {ResourceType::Texture, TextureLoader::SaveBinary},
        {ResourceType::Scene, SceneLoader::SaveBinary},
        {ResourceType::Script, ScriptLoader::SaveBinary},
        {ResourceType::Prefab, PrefabLoader::SaveBinary}
    };

    Reference<Resource> ResourceLoader::Load(const ResourceMetadata& metadata)
	{
		if (metadata.Type == ResourceType::None) {
			TRI_CORE_ERROR("Invalid resource type");
			return nullptr;
		}

		LoadFunction loadfn = s_LoadFunctions.at(metadata.Type);
		return loadfn(metadata);
	}

	void ResourceLoader::Save(Reference<Resource> resource)
	{
		const ResourceMetadata& metadata = resource->MetaData;
		if (metadata.Type == ResourceType::None) {
			TRI_CORE_ERROR("Invalid resource type");
			return;
		}

		SaveFunction savefn = s_SaveFunctions.at(metadata.Type);
		savefn(resource);
	}

    Reference<Resource> ResourceLoader::LoadBinary(const ResourceMetadata& metadata)
    {
		if (metadata.Type == ResourceType::None)
		{
			TRI_CORE_ERROR("Invalid resource type");
			return nullptr;
		}

		LoadFunction loadfn = s_BinaryLoadFunctions.at(metadata.Type);
		return loadfn(metadata);
	}

	void ResourceLoader::SaveBinary(Reference<Resource> resource, std::ostream &stream)
	{
		const ResourceMetadata& metadata = resource->MetaData;
		if (metadata.Type == ResourceType::None)
		{
			TRI_CORE_ERROR("Invalid resource type");
			return;
		}

		SaveBinaryFunction savefn = s_BinarySaveFunctions.at(metadata.Type);
		savefn(resource, stream);
	}
}