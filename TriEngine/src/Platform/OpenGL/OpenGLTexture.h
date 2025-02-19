#pragma once

#include <glm/glm.hpp>
#include "Core/Renderer/Texture.h"
#include "Core/Base/Core.h"

namespace TriEngine {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const ByteBuffer& buffer, const TextureSettings& settings);
		OpenGLTexture2D(const glm::ivec2& size, const TextureSettings& settings);
		OpenGLTexture2D(const std::string& filePath, const TextureSettings& settings);
		OpenGLTexture2D(const glm::vec4& color, uint32_t size, const TextureSettings& settings);
		OpenGLTexture2D(const glm::vec4& startColor, const glm::vec4& endColor, uint32_t size, const TextureSettings& settings);
		virtual ~OpenGLTexture2D() final;

		virtual void Bind(uint32_t slot) const final;
		virtual void UnBind(uint32_t slot) const final;

		virtual bool HasTransparency() const final { return m_Transparent; }
		virtual std::string& GetFilePath() final { return m_Path; }
		virtual TextureFilter GetFilterMode() const final { return m_Settings.Filter; }
		virtual TextureWrap GetWrapMode() const final { return m_Settings.Wrap; }
		virtual const TextureSettings& GetSettings() const final { return m_Settings; }

		virtual ByteBuffer& GetData() final { return m_Buffer; };
		virtual uint32_t GetWidth() const final { return m_Settings.SizeX; };
		virtual uint32_t GetHeight() const final { return m_Settings.SizeY; };

		virtual RID GetID() const final { return m_TextureID; };
		virtual uint64_t GetHandle() const override { return m_BindlessHandle; }
	private:
		RID m_TextureID;
		uint64_t m_BindlessHandle = 0;
		
		TextureSettings m_Settings;

		bool m_Transparent = false;
		ByteBuffer m_Buffer;
		std::string m_Path;
	};
}
