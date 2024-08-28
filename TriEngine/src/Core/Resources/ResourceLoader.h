#pragma once

#include "Resource.h"

namespace TriEngine {
	class ResourceLoader {
	public:
		static Reference<Resource> Load(ResourceMetadata& metadata);
		static void Save(Reference<Resource> metadata);
	};
}
