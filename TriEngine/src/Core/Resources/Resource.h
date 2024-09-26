#pragma once

#include "Core/Base/UUID.h"
#include "Core/Base/Core.h"
#include "string"

namespace TriEngine {

	using ResourceID = UUID;

	enum class ResourceType : uint8_t {
		None = 0,
		Texture,
		Scene
	};

	struct ResourceMetadata {
		ResourceType Type = ResourceType::None;
		ResourceID ID;
		std::string Filepath;
	};

	class Resource {
	public:

		ResourceMetadata MetaData;

		template<typename T>
		Reference<T> As() { return reinterpret_cast<T*>(this); }

		operator bool() const {
			return MetaData.Type != ResourceType::None;
		}

		virtual ~Resource() = default;
	};
}
