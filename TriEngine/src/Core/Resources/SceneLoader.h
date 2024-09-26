#pragma once

#include "Resource.h"

namespace TriEngine {
	class SceneLoader {
	public:
		static Reference<Resource> Load(ResourceMetadata& metadata);
		static void Save(Reference<Resource> resource);
	};
}