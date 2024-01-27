#include "tripch.h"
#include "Buffers.h"

#include "Base/Assert.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffers.h"

namespace TriEngine {
	Reference<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexBuffer>(OpenGLVertexBuffer(vertices, size));

		TRI_CORE_ASSERT(false, "Invalid renderer type!");
		return nullptr;
		}
	}

	Reference<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLIndexBuffer>(OpenGLIndexBuffer(indices, size));

		TRI_CORE_ASSERT(false, "Invalid renderer type!");
		return nullptr;
		}
	}
}