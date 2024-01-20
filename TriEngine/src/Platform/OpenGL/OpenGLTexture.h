#pragma once

#include "Renderer/Texture.h"

namespace TriEngine {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const std::string& filePath);
		virtual ~OpenGLTexture2D() final;

		virtual void Bind() const final;
		virtual void UmBind() const final;

		virtual std::vector<std::byte>& GetData() const final;
		virtual uint32_t GetWidth() const final;
		virtual uint32_t GetHeight() const final;

		virtual uint32_t GetID() const final;
	private:
		uint32_t m_TextureID;

		uint32_t m_Width;
		uint32_t m_Height;
	};
}
