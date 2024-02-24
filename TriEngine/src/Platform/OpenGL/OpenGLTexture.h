#pragma once

#include <glm/glm.hpp>
#include "Core/Renderer/Texture.h"
#include "Core/Base/Core.h"

namespace TriEngine {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const glm::ivec2& size, const TextureSettings& settings);
		OpenGLTexture2D(const std::string& filePath, const TextureSettings& settings);
		OpenGLTexture2D(const glm::vec4& color, uint32_t size, const TextureSettings& settings);
		OpenGLTexture2D(const glm::vec4& startColor, const glm::vec4& endColor, uint32_t size, const TextureSettings& settings);
		virtual ~OpenGLTexture2D() final;

		virtual void Bind(uint32_t slot) const final;
		virtual void UnBind(uint32_t slot) const final;

		virtual TextureFilter GetFilterMode() const final { return m_Settings.Filter; }
		virtual TextureWrap GetWrapMode() const final { return m_Settings.Wrap; }

		virtual ByteBuffer& GetData() final { return m_Buffer; };
		virtual uint32_t GetWidth() const final { return m_Width; };
		virtual uint32_t GetHeight() const final { return m_Height; };

		virtual RID GetID() const final { return m_TextureID; };
	private:
		TextureSettings m_Settings;

		ByteBuffer m_Buffer;
		uint32_t m_Width, m_Height;
		std::string m_Path;

		RID m_TextureID;
	};
}
