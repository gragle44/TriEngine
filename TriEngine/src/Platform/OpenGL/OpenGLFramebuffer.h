#pragma once

#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/Texture.h"

#include <glad/glad.h>

namespace TriEngine {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSettings& settings);
		virtual ~OpenGLFrameBuffer() final;

		virtual void Bind() final;
		virtual void UnBind() final;

		virtual uint32_t GetWidth() final { return m_Settings.Width; }
		virtual uint32_t GetHeight() final { return m_Settings.Height; }

		virtual void ReSize(uint32_t width, uint32_t height) final;

		virtual bool OnWindowResize(WindowResizeEvent& e) final;

		virtual RID GetColorAttachment(uint32_t index) const { return m_ColorAttachments[index]; }
		virtual void BindColorAttachment(uint32_t slot, uint32_t index) const;

		virtual RID GetDepthAttachment() const { return m_DepthAttachment; }
		virtual void BindDepthAttachment(uint32_t slot) const;

		void Recreate();
	private:
		void AttachColorTexture(RID id, GLenum format, int index);
		void AttachDepthTexture(RID id, GLenum format);


		RID m_BufferID = 0;
		FrameBufferSettings m_Settings;

		std::vector<RenderAttachmentSettings> m_ColorAttachmentSettings;
		RenderAttachmentSettings m_DepthAttachmentSettings;

		std::vector<RID> m_ColorAttachments;
		RID m_DepthAttachment = 0;
	};
}
