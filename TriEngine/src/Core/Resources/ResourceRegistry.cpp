#include "tripch.h"

#include "ResourceRegistry.h"
#include "ResourceArchive.h"

#include "Projects/ProjectManager.h"

#include "magic_enum.hpp"

namespace TriEngine {
    ResourceRegistry* ResourceRegistry::Create(const std::filesystem::path& path) {
        if (ProjectManager::GetCurrentProjectData().Binary)
            return new BinaryResourceRegistry(path);
        else
            return new EditorResourceRegistry(path);
    }

    void EditorResourceRegistry::Load()
    {
        if (!std::filesystem::exists(m_RegistryPath))
        {
            TRI_CORE_ERROR("Couldn't find Resource Registry: {0}", m_RegistryPath);
            return;
        }

        auto registry = YAML::LoadAllFromFile(m_RegistryPath.generic_string());

        auto registryMetadata = registry[1];

        for (auto data : registryMetadata)
        {
            ResourceMetadata metadata;
            metadata.ID = data["ID"].as<uint64_t>();
            metadata.Filepath = data["Filepath"].as<std::string>();
            metadata.Type = magic_enum::enum_cast<ResourceType>(data["Type"].as<std::string>()).value_or(ResourceType::None);
            m_Registry[metadata.ID] = metadata;
        }
    }

    void EditorResourceRegistry::Save()
    {
        YAML::Emitter out;

        out << "TriEngine Resource Registry";

        out << YAML::BeginSeq;

        for (const auto &[id, metadata] : m_Registry)
        {
            std::string filepath = metadata.Filepath;

            #ifdef TRI_PLATFORM_WINDOWS
                std::replace(filepath.begin(), filepath.end(), '\\', '/');
            #endif

            out << YAML::BeginMap;
            out << YAML::Key << "ID" << YAML::Value << id;
            out << YAML::Key << "Filepath" << YAML::Value << filepath;
            out << YAML::Key << "Type" << YAML::Value << magic_enum::enum_name(metadata.Type).data();
            out << YAML::EndMap;
        }
        out << YAML::EndSeq;

        std::ofstream fout(m_RegistryPath);
        fout << out.c_str();
    }

    void BinaryResourceRegistry::Load()
    {
        LookupHeader header;

        std::ifstream registry(m_RegistryPath, std::ios::binary);
        registry.seekg(0, std::ios::beg);

        registry.read(reinterpret_cast<char*>(&header), sizeof(header));

        if (header.Magic != *reinterpret_cast<const uint32_t*>(&RESOURCE_HEADER_MAGIC)) {
            TRI_CORE_ERROR("Invalid resource registry file '{}'", m_RegistryPath);
            TRI_CORE_ASSERT(false, "See above error");
            return;
        }

        for (uint32_t i = 0; i < header.ResourceCount; i++) {
            ArchivedResourceMetaData archivedMetadata;
            registry.read(reinterpret_cast<char*>(&archivedMetadata), sizeof(archivedMetadata));

            ResourceMetadata metadata;
            metadata.ArchiveIndex = archivedMetadata.ArchiveIndex;
            metadata.ID = archivedMetadata.ID;
            metadata.ArchiveOffset = archivedMetadata.ArchiveOffset;
            metadata.Type = archivedMetadata.Type;

            m_Registry[metadata.ID] = metadata;
        }
    }

    void BinaryResourceRegistry::Save()
    {
        LookupHeader header;
        memcpy(&header.Magic, &RESOURCE_HEADER_MAGIC, sizeof(uint32_t));
        header.ResourceCount = m_Registry.size();
        header.Version = 0;

        std::ofstream registry(m_RegistryPath, std::ios::binary);
        registry.write(reinterpret_cast<char*>(&header), sizeof(LookupHeader));

        for (const auto& [id, metadata] : m_Registry) {
            ArchivedResourceMetaData archiveMetadata;
            archiveMetadata.ID = id;
            archiveMetadata.Type = metadata.Type;
            archiveMetadata.ArchiveIndex = metadata.ArchiveIndex;
            archiveMetadata.ArchiveOffset = metadata.ArchiveOffset;

            registry.write(reinterpret_cast<char*>(&archiveMetadata), sizeof(ArchivedResourceMetaData));
        } 
    }
}