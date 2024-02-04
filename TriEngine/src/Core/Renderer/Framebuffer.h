#pragma once

#include "Core/Base/Core.h"

namespace TriEngine {
	struct FrameBufferSettings {
		uint32_t Width, Height;

		uint32_t Samples = 1;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		static Reference<FrameBuffer> Create(const FrameBufferSettings& settings);
	};
}