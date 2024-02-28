#pragma once

#include "Core/Base/AssetLibrary.h"
#include "Texture.h"

namespace TriEngine {
	class TextureManager {
	public:
		static Reference<Texture2D> Create2D(const std::string& name, const glm::ivec2& size, const TextureSettings& settings = TextureSettings());

		static Reference<Texture2D> Create2D(const std::string& name, const std::string& filePath, const TextureSettings& settings = TextureSettings());

		static Reference<Texture2D> Create2D(const std::string& name, const glm::vec4& color, uint32_t size, const TextureSettings& settings = TextureSettings());

		static Reference<Texture2D> Create2D(const std::string& name, const glm::vec4& color, const glm::vec4& endColor, uint32_t size, const TextureSettings& settings = TextureSettings());

		static void Use(const std::string& name);
		static Reference<Texture2D> Get(const std::string& name) { return s_TextureLibrary.Get(name); };
		static AssetLibrary<Texture2D>& GetTextureLibrary() { return s_TextureLibrary; }
	private:
		static AssetLibrary<Texture2D> s_TextureLibrary;
	};
}
