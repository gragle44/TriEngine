#pragma once

#include <string>

namespace TriEngine {
	struct RendererCapabilities {
		std::string Vendor;
		std::string Device;
		std::string Version;

		int32_t MaxSamplers = 0;
		int32_t MaxFramebufferSize = 0;
		int32_t MaxColorAttachments = 0;
		int32_t MaxComputeGroupSizeX = 0;
		int32_t MaxComputeGroupSizeY = 0;
		int32_t MaxComputeGroupSizeZ = 0;
	};
}