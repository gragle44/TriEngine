#include "tripch.h"
#include "Buffers.h"

#include "Base/Assert.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffers.h"

namespace TriEngine {
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);

		TRI_CORE_ASSERT(false, "Invalid renderer type!");
		return nullptr;
		}
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);

		TRI_CORE_ASSERT(false, "Invalid renderer type!");
		return nullptr;
		}
	}
}