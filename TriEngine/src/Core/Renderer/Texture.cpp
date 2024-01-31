#include "tripch.h"
#include "Texture.h"

#include "Base/Assert.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace TriEngine {
	Reference<Texture2D> Texture2D::Create(const std::string& filePath)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(filePath);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const glm::vec4& color, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(color, size);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const glm::vec4& startColor, const glm::vec4& endColor, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(startColor, endColor, size);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}
}