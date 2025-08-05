#include "Resource.h"

#include <filesystem>

namespace TriEngine {
    class ResourceRegistry {
    public:
        // Returns the correct type of resource registry based on project settings. Must be freed by the application
        static ResourceRegistry* Create(const std::filesystem::path& path);

        virtual ~ResourceRegistry() = default;

        virtual ResourceMetadata& GetMetaData(ResourceID id) = 0;

        virtual void SetMetaData(ResourceID id, const ResourceMetadata& metadata) = 0;
        virtual void Remove(ResourceID id) = 0;

        virtual bool ResourceExists(ResourceID id) const = 0;

        virtual void Load() = 0;
        virtual void Save() = 0;

        virtual const std::unordered_map<ResourceID, ResourceMetadata>& GetRegistry() const = 0;

        constexpr virtual bool IsBinaryRegistry() const = 0;
    };

    class EditorResourceRegistry : public ResourceRegistry {
    public:
        EditorResourceRegistry(const std::filesystem::path& path)
            :m_RegistryPath(path) {}
        virtual ResourceMetadata& GetMetaData(ResourceID id) override { return m_Registry.at(id); }

        virtual void SetMetaData(ResourceID id, const ResourceMetadata& metadata) override { m_Registry[id] = metadata; }
        virtual void Remove(ResourceID id) override { m_Registry.erase(id); }

        virtual bool ResourceExists(ResourceID id) const override { return m_Registry.contains(id); }

        virtual void Load() override;
        virtual void Save() override;

        virtual const std::unordered_map<ResourceID, ResourceMetadata>& GetRegistry() const override { return m_Registry; }

        constexpr virtual bool IsBinaryRegistry() const override { return false; };

    private:
        std::unordered_map<ResourceID, ResourceMetadata> m_Registry;
        std::filesystem::path m_RegistryPath;
    };

    class BinaryResourceRegistry : public ResourceRegistry
    {
    public:
        BinaryResourceRegistry(const std::filesystem::path& path)
            : m_RegistryPath(path) {}
        virtual ResourceMetadata& GetMetaData(ResourceID id) override { return m_Registry.at(id); }

        virtual void SetMetaData(ResourceID id, const ResourceMetadata &metadata) override { m_Registry[id] = metadata; }
        virtual void Remove(ResourceID id) override { TRI_CORE_ASSERT(false, "Not supported"); }

        virtual bool ResourceExists(ResourceID id) const override { return m_Registry.contains(id); }

        virtual void Load() override;
        virtual void Save() override;

        virtual const std::unordered_map<ResourceID, ResourceMetadata>& GetRegistry() const override { return m_Registry; }

        constexpr virtual bool IsBinaryRegistry() const override { return true; };

    private:
        std::unordered_map<ResourceID, ResourceMetadata> m_Registry;
        std::filesystem::path m_RegistryPath;
    };
}