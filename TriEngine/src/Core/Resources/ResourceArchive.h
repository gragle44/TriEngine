#include "Resource.h"

namespace TriEngine {
    enum class ResourceEncryptionType : uint8_t {
        None = 0
    };

    enum class ChecksumType : uint8_t {
        None = 0
    };

    static constexpr uint8_t RESOURCE_HEADER_MAGIC[] = "TRPK";

    struct LookupHeader {
        uint32_t Magic;
        uint16_t Version;
        uint32_t ResourceCount;
    };

    struct ArchivedResourceMetaData
    {
        uint64_t ID;
        uint32_t ArchiveIndex; // The index of the archive this resource file is contained in
        uint64_t ArchiveOffset; // Offset in bytes of the resource in the archive
        ResourceType Type;
    };

    struct ArchiveHeader {
        uint32_t Magic;
        uint16_t Version;
        uint32_t Index;

        uint32_t FileCount;
    };

    struct ArchivedFileHeader
    {
        uint32_t Size;
        uint32_t CompressedSize;
        uint32_t EncryptedSize;

        ResourceType Type;
    };
};