#include "tripch.h"

#include "ScriptLoader.h"

#include "ResourceArchive.h"

#include "Core/Scripting/Script.h"
#include "Core/Scripting/ScriptEngine.h"

#include "Core/Projects/ProjectManager.h"

#include <filesystem>

namespace TriEngine {
	Reference<Resource> TriEngine::ScriptLoader::Load(const ResourceMetadata& metadata)
	{
		Reference<Script> script = std::make_shared<Script>();
		script->MetaData = metadata;
		script->Name = std::filesystem::path(metadata.Filepath).filename().string();

		ScriptEngine& engine = ScriptEngine::Get();
		engine.BuildScript(script);

		return script;
	}

	void ScriptLoader::Save(Reference<Resource> resource)
	{

	}

	Reference<Resource> ScriptLoader::LoadBinary(const ResourceMetadata& metadata) {
		std::filesystem::path archivePath = ProjectManager::GetCurrent()->GetAbsolutePath(std::format("data/{}.pck", metadata.ArchiveIndex));

		if (!std::filesystem::exists(archivePath)) {
			TRI_CORE_ERROR("Cannot load archive '{}', invalid filepath", archivePath);
			return nullptr;
		}

		ArchivedFileHeader header;
		ByteBuffer byteCode;

		std::ifstream archive(archivePath, std::ios::binary);
		archive.seekg(metadata.ArchiveOffset, std::ios::beg);

		archive.read(reinterpret_cast<char*>(&header), sizeof(header));

		uint32_t byteCodeSize = header.EncryptedSize - sizeof(header);
		byteCode.resize(byteCodeSize);
		archive.read(reinterpret_cast<char*>(byteCode.data()), byteCodeSize);

		archive.close();

		Reference<Script> script = std::make_shared<Script>();
		script->MetaData = metadata;
		script->Name = std::to_string(metadata.ID); // This is normally the file name, but that doesn't exist here
		script->Bytecode = byteCode;
		script->HasDebugInfo = false;

		ScriptEngine& engine = ScriptEngine::Get();
		engine.BuildScript(script);

		return script;
	}

	void ScriptLoader::SaveBinary(Reference<Resource> resource,std::ostream& stream) {
		Reference<Script> script = std::dynamic_pointer_cast<Script>(resource);

		ScriptEngine& engine = ScriptEngine::Get();
		ByteBuffer byteCode = engine.SaveByteCode(script);

		uint32_t size = sizeof(ArchivedFileHeader) + byteCode.size();
		ArchivedFileHeader header;
		header.Type = script->MetaData.Type;
		header.Size = size;
		header.CompressedSize = size;
		header.EncryptedSize = size;
		
		stream.write(reinterpret_cast<char*>(&header), sizeof(header));
		stream.write(reinterpret_cast<char*>(byteCode.data()), byteCode.size());
	}
}
