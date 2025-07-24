#include "tripch.h"

#include "SceneLoader.h"

#include "ResourceArchive.h"

#include "Core/Projects/ProjectManager.h"

#include "GameObjects/Scene.h"
#include "GameObjects/SceneSerializer.h"

namespace TriEngine {
	Reference<Resource> TriEngine::SceneLoader::Load(const ResourceMetadata& metadata)
	{
		Reference<Scene> scene = Scene::Create();
		TriEngine::SceneSerializer s(scene);
		s.Deserialize(metadata.Filepath);
 		scene->MetaData = metadata;
		return scene;
	}

	void SceneLoader::Save(Reference<Resource> resource)
	{
		auto scene = std::dynamic_pointer_cast<Scene>(resource);
		TriEngine::SceneSerializer s(scene);
		s.Serialize(scene->MetaData.Filepath);
	}

	Reference<Resource> TriEngine::SceneLoader::LoadBinary(const ResourceMetadata& metadata)
	{
		std::filesystem::path archivePath = ProjectManager::GetCurrent()->GetAbsolutePath(std::format("data/{}.pck", metadata.ArchiveIndex));
		if (!std::filesystem::exists(archivePath))
		{
			TRI_CORE_ERROR("Cannot load archive '{}', invalid filepath", archivePath);
			return nullptr;
		}

		ArchivedFileHeader header;
		std::string sceneData;

		std::ifstream archive(archivePath, std::ios::binary);
		archive.seekg(metadata.ArchiveOffset, std::ios::beg);

		archive.read(reinterpret_cast<char*>(&header), sizeof(header));
		sceneData.resize(header.EncryptedSize - sizeof(header));
		archive.read(sceneData.data(), header.EncryptedSize - sizeof(header));

		archive.close();

		std::stringstream sceneDataStream;
		sceneDataStream.write(sceneData.data(), sceneData.size());

		Reference<Scene> scene = Scene::Create();

		TriEngine::SceneSerializer s(scene);
		s.Deserialize(sceneDataStream);
		
		scene->MetaData = metadata;
		return scene;
	}

	void SceneLoader::SaveBinary(Reference<Resource> resource, std::ostream& stream)
	{
		Reference<Scene> scene = std::dynamic_pointer_cast<Scene>(resource);

		std::stringstream sceneDataStream;
		SceneSerializer s(scene);
		s.Serialize(sceneDataStream);

		std::string sceneData = sceneDataStream.str();

		const uint32_t size = sizeof(ArchivedFileHeader) + sceneData.size();
		ArchivedFileHeader header;
		header.Type = scene->MetaData.Type;
		header.Size = size;
		header.CompressedSize = size;
		header.EncryptedSize = size;

		stream.write(reinterpret_cast<char*>(&header), sizeof(header));
		stream.write(sceneData.data(), sceneData.size());
	}
}
