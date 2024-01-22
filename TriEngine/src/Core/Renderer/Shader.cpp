#include "tripch.h"
#include "Shader.h"

#include "Base/Assert.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace TriEngine {
	Shader* Shader::Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) {
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(name, vertexPath, fragmentPath);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}
}