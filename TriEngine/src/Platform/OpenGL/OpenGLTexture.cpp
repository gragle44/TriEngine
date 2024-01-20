#include "tripch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace TriEngine {
    OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)
        :m_Path(filePath)
    {
        int x, y, channels;
        stbi_uc* data = stbi_load(filePath.c_str(), &x, &y, &channels, 0);

        const char* failureReason = stbi_failure_reason();
        TRI_CORE_ERROR(failureReason);
        TRI_CORE_ASSERT(data, "Failed to load image texture!");

        m_Width = x;
        m_Height = y;

        uint32_t dataSize = x * y * channels;
        m_Buffer.reserve(dataSize);
        m_Buffer.insert(m_Buffer.end(), reinterpret_cast<std::byte*>(data), reinterpret_cast<std::byte*>(data + dataSize));

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureStorage2D(m_TextureID, 1, GL_RGB8, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer.data());

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_TextureID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_TextureID);
    }

    void OpenGLTexture2D::UnBind(uint32_t slot) const
    {
        glBindTexture(slot, 0);
    }
}