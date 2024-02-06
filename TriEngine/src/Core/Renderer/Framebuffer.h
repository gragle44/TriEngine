#pragma once

#include "Core/Base/Core.h"
#include "Core/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLTexture.h"

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

		virtual bool OnWindowResize(WindowResizeEvent& e) = 0;

		virtual void BindColorAttachment() = 0;
		virtual const Reference<Texture2D>& GetColorAttachment() = 0;

		static Reference<FrameBuffer> Create(const FrameBufferSettings& settings);
	};
}