#include "tripch.h"
#include "OpenGLTexture.h"

#include "Base/Assert.h"

#include <stb_image.h>
#include <glm/vec2.hpp>
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
        default:
            TRI_CORE_ASSERT(false, "Invalid TextureFilter!");
            return GL_NONE;
        }
    }

    static GLenum WrapModeToOpenGLEnum(TextureWrap wrap) {
        switch (wrap)
        {
        case TriEngine::TextureWrap::None:
            TRI_CORE_ASSERT(false, "TextureWrap was None!");
            return GL_NONE;
        case TriEngine::TextureWrap::Repeat:
            return GL_REPEAT;
        case TriEngine::TextureWrap::MirroredRepeat:
            return GL_MIRRORED_REPEAT;
        case TriEngine::TextureWrap::ClampEdge:
            return GL_CLAMP_TO_EDGE;
        case TriEngine::TextureWrap::ClampBorder:
            return GL_CLAMP_TO_BORDER;
        default:
            TRI_CORE_ASSERT(false, "Invalid TextureWrap!");
            return GL_NONE;
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const glm::ivec2& size, const TextureSettings& settings)
        :m_Width(size.x), m_Height(size.y), m_Settings(settings)
    {
        GLenum openGLFormat = 0, dataFormat = 0;

        switch (settings.Usage)
        {
        case TriEngine::TextureUsage::Depth: 
            openGLFormat = GL_DEPTH24_STENCIL8; 
            dataFormat = GL_DEPTH_STENCIL; 
            break;
        case TriEngine::TextureUsage::Image: 
            openGLFormat = GL_RGBA8; 
            dataFormat = GL_RGBA; 
            break;
        }

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        GLenum filter = FilterModeToOpenGLEnum(settings.Filter);
        GLenum wrap = WrapModeToOpenGLEnum(settings.Wrap);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, wrap);

        if (settings.Samples > 1) {
            glTextureStorage2DMultisample(m_TextureID, settings.Samples, openGLFormat, m_Width, m_Height, GL_TRUE);
        }
        else {
            glTextureStorage2D(m_TextureID, 1, openGLFormat, m_Width, m_Height);
        }
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& filePath, const TextureSettings& settings)
        :m_Path(filePath), m_Settings(settings)
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

        GLenum filter = FilterModeToOpenGLEnum(settings.Filter);
        GLenum wrap = WrapModeToOpenGLEnum(settings.Wrap);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, wrap);

        glTextureStorage2D(m_TextureID, 1, openGLFormat, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::OpenGLTexture2D(const glm::vec4& color, uint32_t size, const TextureSettings& settings)
        :m_Settings(settings), m_Width(size), m_Height(size)
    {
        uint8_t colorBytes[] = {
            (uint8_t)(color.r * 0xffu),
            (uint8_t)(color.g * 0xffu),
            (uint8_t)(color.b * 0xffu),
            (uint8_t)(color.a * 0xffu) 
        };

        m_Buffer.resize(m_Width * m_Height * sizeof(colorBytes));
        m_Buffer.assign(reinterpret_cast<uint8_t*>(colorBytes), reinterpret_cast<uint8_t*>(colorBytes) + m_Buffer.size());

        GLenum openGLFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        GLenum filter = FilterModeToOpenGLEnum(settings.Filter);
        GLenum wrap = WrapModeToOpenGLEnum(settings.Wrap);

        glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, filter);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, wrap);
        glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, wrap);

        glTextureStorage2D(m_TextureID, 1, openGLFormat, m_Width, m_Height);
        glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, m_Buffer.data());

    }

    OpenGLTexture2D::OpenGLTexture2D(const glm::vec4& startColor, const glm::vec4& endColor, uint32_t size, const TextureSettings& settings)
        :m_Settings(settings), m_Width(size), m_Height(size)
    {
        m_Buffer.resize(m_Width * m_Height * sizeof(uint8_t) * 4);
        
        auto iterator = m_Buffer.begin();

        for (uint32_t y = 0; y < size; y++)
        {
            glm::vec4 rowColor = glm::mix(startColor, endColor, (float)y / (float)size);

            uint8_t colourBytes[] = {
                (uint8_t)(rowColor.r * 0xffu),
                (uint8_t)(rowColor.g * 0xffu),
                (uint8_t)(rowColor.b * 0xffu),
                (uint8_t)(rowColor.a * 0xffu) 
            };

            for (uint32_t x = 0; x < size; ++x)
            {
                memcpy(&*iterator, colourBytes, sizeof(colourBytes));
                iterator += 4;
            }
        }

        GLenum openGLFormat = GL_RGBA8;
        GLenum dataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

        GLenum filter = FilterModeToOpenGLEnum(settings.Filter);
        GLenum wrap = WrapModeToOpenGLEnum(settings.Wrap);

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
        glBindTextureUnit(slot, 0);
    }
}