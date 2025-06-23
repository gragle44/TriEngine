#include "tripch.h"

#include "ScriptLoader.h"

#include "Core/Scripting/Script.h"

#include <filesystem>

namespace TriEngine {
	Reference<Resource> TriEngine::ScriptLoader::Load(ResourceMetadata& metadata)
	{
		Script script;
		script.Name = std::filesystem::path(metadata.Filepath).filename();
		script.MetaData = metadata;

		return std::make_shared<Script>(script);
	}

	void ScriptLoader::Save(Reference<Resource> resource)
	{
		
	}
}
