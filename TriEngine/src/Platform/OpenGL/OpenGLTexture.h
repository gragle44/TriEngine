#pragma once

#include "Renderer/Texture.h"
#include "Core/Core.h"

namespace TriEngine {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& filePath);
		virtual ~OpenGLTexture2D() final;

		virtual void Bind(uint32_t slot) const final;
		virtual void UnBind(uint32_t slot) const final;

		virtual ByteBuffer& GetData() final { return m_Buffer; };
		virtual uint32_t GetWidth() const final { return m_Width; };
		virtual uint32_t GetHeight() const final { return m_Height; };

		virtual uint32_t GetID() const final { return m_TextureID; };
	private:
		uint32_t m_TextureID;
		
		ByteBuffer m_Buffer = std::vector<std::byte>({'e'});
		uint32_t m_Width, m_Height;
		std::string m_Path;
	};
}
