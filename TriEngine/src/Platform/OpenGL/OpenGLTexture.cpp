#include "tripch.h"
#include "OpenGLTexture.h"

#include "Base/Assert.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace TriEngine {
    static GLenum FilterModeToOpenGLEnum(TextureFilter filter) {
        switch (filter)
        {
        case TriEngine::TextureFilter::None:
            TRI_CORE_ASSERT(false, "TextureFilter was None!");
            return GL_NONE;
        case TriEngine::TextureFilter::Linear:
            return GL_LINEAR;
        case TriEngine::TextureFilter::Nearest:
            return GL_NEAREST;
        }
    }

    static GLenum WrapModeToOpenGLEnum(TextureWrapMode wrap) {
        switch (wrap)
        {
        case TriEngine::TextureWrapMode::None:
            TRI_CORE_ASSERT(false, "TextureWrapMode was None!");
            return GL_NONE;
        case TriEngine::TextureWrapMode::Repeat:
            return GL_REPEAT;
        case TriEngine::TextureWrapMode::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
        case TriEngine::TextureWrapMode::ClampEdge:
            return GL_CLAMP_TO_EDGE;
        case TriEngine::TextureWrapMode::ClampBorder:
            return GL_CLAMP_TO_BORDER;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, TextureFilter filterMode, TextureWrapMode wrapMode)
        :m_Path(filePath), m_FilterMode(filterMode), m_WrapMode(wrapMode)
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

        GLenum filter = FilterModeToOpenGLEnum(filterMode);
        GLenum wrap = WrapModeToOpenGLEnum(wrapMode);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, wrap);

        glTextureStorage2D(m_TextureID, 1, openGLFormat, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(const glm::vec4& color, uint32_t size, TextureFilter filterMode, TextureWrapMode wrapMode)
        :m_FilterMode(filterMode), m_WrapMode(wrapMode), m_Width(size), m_Height(size)
    {
        uint8_t colorBytes[] = {
            color.r * 0xff,
            color.g * 0xff,
            color.b * 0xff,
            color.a * 0xff 
        };

        m_Buffer.resize(m_Width * m_Height * sizeof(colorBytes));
        m_Buffer.assign(reinterpret_cast<uint8_t*>(colorBytes), reinterpret_cast<uint8_t*>(colorBytes) + m_Buffer.size());

        GLenum openGLFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;


        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        GLenum filter = FilterModeToOpenGLEnum(filterMode);
        GLenum wrap = WrapModeToOpenGLEnum(wrapMode);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, wrap);

        glTextureStorage2D(m_TextureID, 1, openGLFormat, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, m_Buffer.data());

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