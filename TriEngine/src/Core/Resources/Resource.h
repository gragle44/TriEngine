#pragma once

#include "Core/Base/UUID.h"
#include "Core/Base/Core.h"
#include "string"

namespace TriEngine {

	using ResourceID = UUID;

	enum class ResourceType {
		None,
		Texture
	};

	struct ResourceMetadata {
		ResourceType Type;
		ResourceID ID;
		std::string Filepath;
	};

	class Resource {
	public:

		ResourceMetadata MetaData;

		template<typename T>
		Reference<T> As() { return reinterpret_cast<T*>(this); }

		virtual ~Resource() = default;
	};
}
