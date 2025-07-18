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
		Script
	};

	struct ResourceMetadata {
		ResourceType Type = ResourceType::None;
		ResourceID ID = 0;
		std::string Filepath = "";
	};

	class Resource {
	public:
		ResourceMetadata MetaData;

		template<typename T>
		Reference<T> As() { return reinterpret_cast<T*>(this); }

		operator bool() const {
			return MetaData.Type != ResourceType::None;
		}

		bool operator==(const Resource& other) const {
			return MetaData.ID == other.MetaData.ID;
		}

		Resource() = default;
		Resource(const Resource& other) = default;
		virtual ~Resource() = default;
	};
}
