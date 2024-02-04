#pragma once

#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/Texture.h"

namespace TriEngine {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSettings& settings);
		virtual ~OpenGLFrameBuffer();

		virtual void Bind() final;
		virtual void UnBind() final;

		void Recreate();
	private:
		RID m_BufferID;

		RID m_ColorTexture, m_DepthTexture, m_StencilTexture;

		FrameBufferSettings m_Settings;
	};
}
