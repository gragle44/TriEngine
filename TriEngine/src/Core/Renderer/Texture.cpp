#include "tripch.h"
#include "Texture.h"

#include "Base/Assert.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace TriEngine {
	Reference<Texture2D> Texture2D::Create(const glm::ivec2& size, TextureUsage usage, TextureFilter filterMode, TextureWrap wrapMode)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(size, usage, filterMode, wrapMode);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const std::string& filePath, TextureUsage usage, TextureFilter filterMode, TextureWrap wrapMode)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(filePath, usage, filterMode, wrapMode);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const glm::vec4& color, uint32_t size, TextureUsage usage, TextureFilter filterMode, TextureWrap wrapMode)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(color, size, usage, filterMode, wrapMode);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const glm::vec4& startColor, const glm::vec4& endColor, uint32_t size, TextureUsage usage, TextureFilter filterMode, TextureWrap wrapMode)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(startColor, endColor, size, usage, filterMode, wrapMode);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}
}