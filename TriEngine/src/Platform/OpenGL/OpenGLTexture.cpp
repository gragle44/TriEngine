#include "tripch.h"
#include "OpenGLTexture.h"

#include "Base/Assert.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace TriEngine {
    OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath)
        :m_Path(filePath)
    {
        stbi_set_flip_vertically_on_load(1);


        int x, y, channels;
        stbi_uc* data = stbi_load(filePath.c_str(), &x, &y, &channels, 0);

        if (!data) {
            const char* failureReason = stbi_failure_reason();
            TRI_CORE_ERROR("Failed to load image \"{0}\": {1}", filePath, failureReason);
            TRI_CORE_ASSERT(data, "Image data is null!");
        }

        m_Width = x;
        m_Height = y;

        GLenum openGLFormat = 0;
        GLenum dataFormat = 0;

        if (channels == 3) {
            openGLFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        else if (channels == 4) {
            openGLFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTextureStorage2D(m_TextureID, 1, openGLFormat, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(const glm::vec4& color, uint32_t size)
    {
        //TODO: Create a solid color texture
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