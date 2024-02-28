#include "tripch.h"

#include "TextureManager.h"

namespace TriEngine{
	AssetLibrary<Texture2D> TextureManager::s_TextureLibrary = AssetLibrary<Texture2D>();
	Reference<Texture2D> TextureManager::Create2D(const std::string& name, const glm::ivec2& size, const TextureSettings& settings)
	{
		return Reference<Texture2D>();
	}
	Reference<Texture2D> TextureManager::Create2D(const std::string& name, const std::string& filePath, const TextureSettings& settings)
	{
		return Reference<Texture2D>();
	}
	Reference<Texture2D> TextureManager::Create2D(const std::string& name, const glm::vec4& color, uint32_t size, const TextureSettings& settings)
	{
		return Reference<Texture2D>();
	}
	Reference<Texture2D> TextureManager::Create2D(const std::string& name, const glm::vec4& color, const glm::vec4& endColor, uint32_t size, const TextureSettings& settings)
	{
		return Reference<Texture2D>();
	}
	void TextureManager::Use(const std::string& name)
	{
	}
}