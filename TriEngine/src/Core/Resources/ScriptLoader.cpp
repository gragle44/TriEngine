#include "tripch.h"

#include "ScriptLoader.h"

#include "Core/Scripting/Script.h"
#include "Core/Scripting/ScriptEngine.h"

#include <filesystem>

namespace TriEngine {
	Reference<Resource> TriEngine::ScriptLoader::Load(ResourceMetadata& metadata)
	{
		Reference<Script> script = std::make_shared<Script>();
		script->MetaData = metadata;
		script->Name = std::filesystem::path(metadata.Filepath).filename().string();

		ScriptEngine& engine = ScriptEngine::Get();
		engine.CompileScript(script.get());

		return script;
	}

	void ScriptLoader::Save(Reference<Resource> resource)
	{
		
	}
}
