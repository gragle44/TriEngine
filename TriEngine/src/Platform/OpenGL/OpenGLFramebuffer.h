#pragma once

#include "Core/Renderer/Framebuffer.h"
#include "Core/Renderer/Texture.h"

#include <glad/glad.h>

namespace TriEngine {
	class OpenGLFrameBuffer : public FrameBuffer {
	public:
		OpenGLFrameBuffer(const FrameBufferSettings& settings);
		virtual ~OpenGLFrameBuffer() override;

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual uint32_t GetWidth() override { return m_Settings.Width; }
		virtual uint32_t GetHeight() override { return m_Settings.Height; }

		virtual int32_t ReadPixel(uint32_t attachment, int32_t x, int32_t y) override;

		virtual void ReSize(uint32_t width, uint32_t height) override;

		virtual bool OnWindowResize(WindowResizeEvent& e) override;

		virtual RID GetColorAttachment(uint32_t index) const override { return m_ColorAttachments[index]; }
		virtual void BindColorAttachment(uint32_t slot, uint32_t index) const override;
		virtual void ClearColorAttachment(uint32_t index, float clearValue = 0.0f) const override;

		virtual RID GetDepthAttachment() const override { return m_DepthAttachment; }
		virtual void BindDepthAttachment(uint32_t slot) const override;

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
