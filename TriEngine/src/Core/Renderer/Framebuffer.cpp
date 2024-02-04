#include "tripch.h"
#include "Framebuffer.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace TriEngine {
    Reference<FrameBuffer> TriEngine::FrameBuffer::Create(const FrameBufferSettings& settings)
    {
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLFrameBuffer>(settings);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
    }
}
