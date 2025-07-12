#include "tripch.h"
#include "Shader.h"

#include "Base/Assert.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace TriEngine {
	Reference<Shader> Shader::Create(std::string_view name, std::string_view vertexSource, std::string_view fragmentSource) {
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<ComputeShader> ComputeShader::Create(std::string_view name, std::string_view source)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLComputeShader>(name, source);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}
}