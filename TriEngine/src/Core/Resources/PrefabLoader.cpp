#include "tripch.h"

#include "PrefabLoader.h"

#include "ResourceArchive.h"

#include "Core/Projects/ProjectManager.h"
#include "ResourceManager.h"

#include "GameObjects/Prefab.h"
#include "GameObjects/SceneSerializer.h"

namespace TriEngine {
    Reference<Resource> TriEngine::PrefabLoader::Load(const ResourceMetadata& metadata) {
        std::ifstream file(ResourceManager::GetAbsolutePath(metadata.Filepath));
        Reference<Prefab> prefab = std::make_shared<Prefab>(file);
        prefab->MetaData = metadata;
        return prefab;
    }

    void PrefabLoader::Save(Reference<Resource> resource) {
        auto prefab = std::dynamic_pointer_cast<Prefab>(resource);
        std::ofstream file(ResourceManager::GetAbsolutePath(prefab->MetaData.Filepath));
        prefab->Save(file);
    }

    Reference<Resource> TriEngine::PrefabLoader::LoadBinary(const ResourceMetadata& metadata) {
        std::filesystem::path archivePath = ProjectManager::GetCurrent()->GetAbsolutePath(std::format("data/{}.pck", metadata.ArchiveIndex));
        if (!std::filesystem::exists(archivePath)) {
            TRI_CORE_ERROR("Cannot load archive '{}', invalid filepath", archivePath);
            return nullptr;
        }

        ArchivedFileHeader header;
        std::string prefabData;

        std::ifstream archive(archivePath, std::ios::binary);
        archive.seekg(metadata.ArchiveOffset, std::ios::beg);

        archive.read(reinterpret_cast<char*>(&header), sizeof(header));
        prefabData.resize(header.EncryptedSize - sizeof(header));
        archive.read(prefabData.data(), header.EncryptedSize - sizeof(header));

        archive.close();

        std::stringstream prefabDataStream;
        prefabDataStream.write(prefabData.data(), prefabData.size());

        Reference<Prefab> prefab = std::make_shared<Prefab>(prefabDataStream);

        prefab->MetaData = metadata;
        return prefab;
    }

    void PrefabLoader::SaveBinary(Reference<Resource> resource, std::ostream& stream) {
        Reference<Prefab> prefab = std::dynamic_pointer_cast<Prefab>(resource);

        std::stringstream dataStream;
        prefab->Save(dataStream);

        std::string data = dataStream.str();

        const uint32_t size = sizeof(ArchivedFileHeader) + data.size();
        ArchivedFileHeader header;
        header.Type = prefab->MetaData.Type;
        header.Size = size;
        header.CompressedSize = size;
        header.EncryptedSize = size;

        stream.write(reinterpret_cast<char*>(&header), sizeof(header));
        stream.write(data.data(), data.size());
    }
}
