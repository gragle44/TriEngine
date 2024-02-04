#include "tripch.h"
#include "OpenGLFramebuffer.h"
#include "Base/Assert.h"

#include <glad/glad.h>

namespace TriEngine {
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSettings& settings)
        :m_Settings(settings)
    {
        Recreate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_BufferID);
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::Recreate()
    {
        glCreateFramebuffers(1, &m_BufferID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorTexture);

        glTextureParameteri(m_ColorTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_ColorTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_ColorTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_ColorTexture, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);

        glTextureStorage2D(m_ColorTexture, 1, GL_RGBA8, m_Settings.Width, m_Settings.Height);
        glTextureSubImage2D(m_ColorTexture, 0, 0, 0, m_Settings.Width, m_Settings.Height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthTexture);

        glTextureStorage2D(m_ColorTexture, 1, GL_DEPTH24_STENCIL8, m_Settings.Width, m_Settings.Height);
        //glTextureSubImage2D(m_ColorTexture, 0, 0, 0, m_Settings.Width, m_Settings.Height, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            TRI_CORE_ERROR("Incomplete FrameBuffer!");
            TRI_CORE_ASSERT(false);
        }
    }
}