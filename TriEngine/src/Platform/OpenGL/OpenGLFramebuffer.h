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

		virtual bool OnWindowResize(WindowResizeEvent& e) final;

		virtual void BindColorAttachment() final;

		void Recreate();
	private:
		RID m_BufferID;

		RID m_RenderBuffer;

		Reference<Texture2D> m_ColorTarget, m_DepthTarget;

		FrameBufferSettings m_Settings;
	};
}
