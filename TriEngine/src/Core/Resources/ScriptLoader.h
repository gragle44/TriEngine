#pragma once

#include "Resource.h"

namespace TriEngine {
	class ScriptLoader {
	public:
		static Reference<Resource> Load(ResourceMetadata& metadata);
		static void Save(Reference<Resource> resource);
	};
}