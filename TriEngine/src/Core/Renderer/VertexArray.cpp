#include "tripch.h"
#include "VertexArray.h"

#include "Base/Assert.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace TriEngine {
	Reference<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLVertexArray>(OpenGLVertexArray());

		TRI_CORE_ASSERT(false, "Invalid renderer type!");
		return nullptr;
		}
	}
}