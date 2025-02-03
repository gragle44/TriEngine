#include "tripch.h"


#include "ShaderStorageBuffer.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLShaderStorageBuffer.h"

namespace TriEngine {
    Reference<ShaderStorageBuffer> TriEngine::ShaderStorageBuffer::Create(const void* data, uint32_t size)
    {
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShaderStorageBuffer>(data, size);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
    }
}
