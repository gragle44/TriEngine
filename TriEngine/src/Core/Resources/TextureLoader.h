#pragma once

#include "Resource.h"

#include "Core/Renderer/Texture.h"

namespace TriEngine {
	class TextureLoader {
	public:
		static Reference<Resource> Load(const ResourceMetadata& metadata);
		static void Save(Reference<Resource> resource);

		static void SaveBinary(Reference<Resource> resource, std::ostream& stream);
		static Reference<Resource> LoadBinary(const ResourceMetadata& metadata);
	};
}
