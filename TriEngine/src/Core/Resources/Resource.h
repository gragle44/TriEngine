#pragma once

#include "Core/Base/UUID.h"
#include "Core/Base/Core.h"
#include "string"

namespace TriEngine {

	using ResourceID = UUID;

	enum class ResourceType : uint8_t {
		None = 0,
		Texture,
		Scene,
		Script,
		Prefab
	};

	struct ResourceMetadata {
		ResourceID ID = 0;
		ResourceType Type = ResourceType::None;
		std::string Filepath = ""; // Filepath of the resource or filepath of the archive it is stored
		uint32_t ArchiveIndex = 0;	// The index of the archive this resource file is contained in, binary only
		uint64_t ArchiveOffset; // Offset in bytes of the resource in the archive, binary only
	};

	class Resource {
	public:
		Resource() = default;
		Resource(const Resource& other) = default;
		virtual ~Resource() = default;

		operator bool() const {
			return MetaData.Type != ResourceType::None;
		}

		bool operator==(const Resource& other) const {
			return MetaData.ID == other.MetaData.ID;
		}

		ResourceMetadata MetaData;
	};
}
