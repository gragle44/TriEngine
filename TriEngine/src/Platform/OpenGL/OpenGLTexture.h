#pragma once

#include <glm/glm.hpp>
#include "Renderer/Texture.h"
#include "Core/Base/Core.h"

namespace TriEngine {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& filePath, TextureFilter filterMode = TextureFilter::Nearest, TextureWrapMode wrapMode = TextureWrapMode::Repeat);
		OpenGLTexture2D(const glm::vec4& color, uint32_t size, TextureFilter filterMode = TextureFilter::Nearest, TextureWrapMode wrapMode = TextureWrapMode::Repeat);
		virtual ~OpenGLTexture2D() final;

		virtual void Bind(uint32_t slot) const final;
		virtual void UnBind(uint32_t slot) const final;

		virtual TextureFilter GetFilterMode() const final { return m_FilterMode; }
		virtual TextureWrapMode GetWrapMode() const final { return m_WrapMode; }

		virtual ByteBuffer& GetData() final { return m_Buffer; };
		virtual uint32_t GetWidth() const final { return m_Width; };
		virtual uint32_t GetHeight() const final { return m_Height; };

		virtual uint32_t GetID() const final { return m_TextureID; };
	private:
		uint32_t m_TextureID;
		
		TextureFilter m_FilterMode;
		TextureWrapMode m_WrapMode;

		ByteBuffer m_Buffer;
		uint32_t m_Width, m_Height;
		std::string m_Path;
	};
}
