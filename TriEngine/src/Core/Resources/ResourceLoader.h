#pragma once

#include "Resource.h"

namespace TriEngine {
	class ResourceLoader {
	public:
		static Reference<Resource> Load(const ResourceMetadata& metadata);
		static void Save(Reference<Resource> resource);

		static Reference<Resource> LoadBinary(const ResourceMetadata& metadata);
		static void SaveBinary(Reference<Resource> resource, std::ostream& stream);
	};
}
