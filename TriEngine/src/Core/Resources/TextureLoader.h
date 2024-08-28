#pragma once

#include "Resource.h"

#include "Core/Renderer/Texture.h"

namespace TriEngine {
	class TextureLoader {
	public:
		static Reference<Resource> Load(ResourceMetadata& metadata);
		static void Save(Reference<Resource> resource);
	};
}
