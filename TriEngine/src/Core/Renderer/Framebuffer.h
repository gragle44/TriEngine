#pragma once

#include "Core/Base/Core.h"
#include "Core/Events/ApplicationEvent.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace TriEngine {
	enum class RenderAttachmentType : uint8_t {
		None = 0,
		Color,
		DepthStencil,
		Depth
	};

	struct RenderAttachmentSettings {
		RenderAttachmentType Type = RenderAttachmentType::None;

		RenderAttachmentSettings() = default;
		RenderAttachmentSettings(RenderAttachmentType type)
			:Type(type) {}
	};

	struct FrameBufferSettings {
		std::initializer_list<RenderAttachmentSettings> Attachments;
		uint32_t Width, Height;

		uint32_t Samples = 1;
	};

	class FrameBuffer {
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void ReSize(uint32_t width, uint32_t height) = 0;

		virtual bool OnWindowResize(WindowResizeEvent& e) = 0;

		virtual RID GetColorAttachment(uint32_t index) const = 0;
		virtual void BindColorAttachment(uint32_t slot, uint32_t index) const = 0;

		virtual RID GetDepthAttachment() const = 0;
		virtual void BindDepthAttachment(uint32_t slot) const = 0;

		static Reference<FrameBuffer> Create(const FrameBufferSettings& settings);
	};
}