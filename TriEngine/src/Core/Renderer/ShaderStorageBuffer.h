#pragma once

#include "Core/Base/Core.h"

namespace TriEngine {
	class ShaderStorageBuffer {
	public:
		virtual ~ShaderStorageBuffer() {}

		virtual void Bind(uint32_t slot) const = 0;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static Reference<ShaderStorageBuffer> Create(const void* data, uint32_t size);
	};
}
