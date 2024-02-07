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

		virtual uint32_t GetWidth() final { return m_Settings.Width; }
		virtual uint32_t GetHeight() final { return m_Settings.Height; }

		virtual void ReSize(uint32_t width, uint32_t height) final;

		virtual bool OnWindowResize(WindowResizeEvent& e) final;

		virtual void BindColorAttachment() final;
		virtual const Reference<Texture2D>& GetColorAttachment() final { return m_ColorTarget; }

		void Recreate();
	private:
		RID m_BufferID = 0;

		RID m_RenderBuffer = 0;

		Reference<Texture2D> m_ColorTarget, m_DepthTarget;

		FrameBufferSettings m_Settings;
	};
}
