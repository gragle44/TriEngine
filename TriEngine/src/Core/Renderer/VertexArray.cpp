#include "tripch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace TriEngine {
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();

		TRI_CORE_ASSERT(false, "Invalid renderer type!");
		return nullptr;
		}
	}
}