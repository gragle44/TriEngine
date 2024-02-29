#include "tripch.h"
#include "Texture.h"

#include "Base/Assert.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace TriEngine {
	Reference<Texture2D> Texture2D::Create(const glm::ivec2& size, const TextureSettings& settings)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(size, settings);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const std::string& filePath, const TextureSettings& settings)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(filePath, settings);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const glm::vec4& color, uint32_t size, const TextureSettings& settings)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(color, size, settings);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	Reference<Texture2D> Texture2D::Create(const glm::vec4& startColor, const glm::vec4& endColor, uint32_t size, const TextureSettings& settings)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::API::None:
			TRI_CORE_ASSERT(false, "Renderer type 'None' is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(startColor, endColor, size, settings);

			TRI_CORE_ASSERT(false, "Invalid renderer type!");
			return nullptr;
		}
	}

	std::string TextureWrapToString(TextureWrap wrap) {
		switch (wrap)
		{
		case TriEngine::TextureWrap::None:
			TRI_CORE_ASSERT(false, "TextureWrap was None!");
			return "None";
		case TriEngine::TextureWrap::Repeat:
			return "Repeat";
		case TriEngine::TextureWrap::MirroredRepeat:
			return "Mirrored Repeat";
		case TriEngine::TextureWrap::ClampEdge:
			return "Clamp to Edge";
		case TriEngine::TextureWrap::ClampBorder:
			return "Clamp to Border";
		default:
			TRI_CORE_ASSERT(false, "Invalid TextureWrap!");
			return "None";
		}
	}

	std::string TextureFilterToString(TextureFilter filter)
	{
		switch (filter)
		{
		case TriEngine::TextureFilter::None:
			TRI_CORE_ASSERT(false, "TextureFilter was None!");
			return "None";
		case TriEngine::TextureFilter::Linear:
			return "Bilinear";
		case TriEngine::TextureFilter::Nearest:
			return "Nearest-neighbor";
		default:
			TRI_CORE_ASSERT(false, "Invalid TextureFilter!");
			return "None";
		}
	}
}