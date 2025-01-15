#include "tripch.h"
#include "OpenGLFramebuffer.h"
#include "Base/Assert.h"
#include "Base/Application.h"
#include "Core/Renderer/RenderCommand.h"


namespace TriEngine {

    static GLenum AttachmentTypeToGLType(RenderAttachmentType type) {
        switch (type)
        {
        case TriEngine::RenderAttachmentType::Color: return GL_RGBA;
        case TriEngine::RenderAttachmentType::DepthStencil: return GL_DEPTH_STENCIL;
        case TriEngine::RenderAttachmentType::Depth: return GL_DEPTH_COMPONENT;
        case TriEngine::RenderAttachmentType::RedInteger:  return GL_RED_INTEGER;
        default: return GL_NONE;
        }
    }

    void OpenGLFrameBuffer::AttachColorTexture(RID id, GLenum format, int index)
    {
        bool multisampled = m_Settings.Samples > 1;
        if (multisampled)
        {
            glTextureStorage2DMultisample(id, m_Settings.Samples, format, m_Settings.Width, m_Settings.Height, GL_FALSE);
        }
        else
        {
            glTextureStorage2D(id, 1, format, m_Settings.Width, m_Settings.Height);

            glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glNamedFramebufferTexture(m_BufferID , GL_COLOR_ATTACHMENT0 + index, id, 0);
    }

    void OpenGLFrameBuffer::AttachDepthTexture(RID id, GLenum format)
    {
        bool multisampled = m_Settings.Samples > 1;
        if (multisampled)
        {
            glTextureStorage2DMultisample(id, m_Settings.Samples, format, m_Settings.Width, m_Settings.Height, GL_FALSE);
        }
        else
        {
            glTextureStorage2D(id, 1, format, m_Settings.Width, m_Settings.Height);

            glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        glNamedFramebufferTexture(m_BufferID, GL_DEPTH_STENCIL_ATTACHMENT, id, 0);
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSettings& settings)
        :m_Settings(settings)
    {
        for (auto setting : m_Settings.Attachments) {
            switch (setting.Type)
            {
            case RenderAttachmentType::Color:
                m_ColorAttachmentSettings.emplace_back(setting); break;
            case RenderAttachmentType::RedInteger:
                m_ColorAttachmentSettings.emplace_back(setting); break;
            case RenderAttachmentType::Depth:
                m_DepthAttachmentSettings = setting; break;
            case RenderAttachmentType::DepthStencil:
                m_DepthAttachmentSettings = setting; break;
            }
        }

        Recreate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_BufferID);
        glDeleteTextures(1, &m_DepthAttachment);
        glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    }

    void OpenGLFrameBuffer::Bind()
    {
        GLenum drawbuffers[8];

        for (int32_t index = 0; index < m_ColorAttachments.size(); index++) {
            drawbuffers[index] = GL_COLOR_ATTACHMENT0 + index;
        }

        glNamedFramebufferDrawBuffers(m_BufferID, m_ColorAttachments.size(), drawbuffers);

        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
        glViewport(0, 0, m_Settings.Width, m_Settings.Height);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int32_t OpenGLFrameBuffer::ReadPixel(uint32_t attachment, int32_t x, int32_t y)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);

        glNamedFramebufferReadBuffer(m_BufferID, GL_COLOR_ATTACHMENT0 + attachment);

        int32_t buff;
        glReadnPixels(x, y, 1, 1, AttachmentTypeToGLType(m_ColorAttachmentSettings[attachment].Type), GL_INT, sizeof(buff), &buff);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return buff;
    }

    void OpenGLFrameBuffer::ReSize(uint32_t width, uint32_t height)
    {
        //TODO: Move to RenderCommand
        uint32_t maxSize = RenderCommand::GetCapabilities().MaxFramebufferSize;

        if (width <= 0 || height <= 0)
            return;
        else if (width > maxSize || height > maxSize)
            return;

        m_Settings.Width = width;
        m_Settings.Height = height;

        Recreate();
    }

    bool OpenGLFrameBuffer::OnWindowResize(WindowResizeEvent& e)
    {
        ReSize(e.GetWidth(), e.GetHeight());

        return false;
    }

    void OpenGLFrameBuffer::BindColorAttachment(uint32_t slot, uint32_t index) const
    {
        glBindTextureUnit(slot, m_ColorAttachments[index]);
    }

    void OpenGLFrameBuffer::ClearColorAttachment(uint32_t index, float clearValue) const
    {
        const auto& settings = m_ColorAttachmentSettings[index];
        glClearTexImage(m_ColorAttachments[index], 0, AttachmentTypeToGLType(settings.Type), GL_INT, &clearValue);
    }


    void OpenGLFrameBuffer::BindDepthAttachment(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_DepthAttachment);
    }

    void OpenGLFrameBuffer::Recreate()
    {
        if (m_BufferID) {
            glDeleteFramebuffers(1, &m_BufferID);
            glDeleteTextures(1, &m_DepthAttachment);
            glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
        }

        glCreateFramebuffers(1, &m_BufferID);

        bool multisample = m_Settings.Samples > 1;

        if (!m_ColorAttachmentSettings.empty()) {
            m_ColorAttachments.resize(m_ColorAttachmentSettings.size());
            glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_ColorAttachmentSettings.size(), m_ColorAttachments.data());

            for (size_t i = 0; i < m_ColorAttachments.size(); i++)
            {
                switch (m_ColorAttachmentSettings[i].Type)
                {
                case RenderAttachmentType::Color:
                    AttachColorTexture(m_ColorAttachments[i], GL_RGBA8, i); break;
                case RenderAttachmentType::RedInteger:
                    AttachColorTexture(m_ColorAttachments[i], GL_R32I, i); break;
                }

            }
        }

        if (m_DepthAttachmentSettings.Type != RenderAttachmentType::None) {
            glCreateTextures(multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_ColorAttachmentSettings.size(), &m_DepthAttachment);

            AttachDepthTexture(m_DepthAttachment, GL_DEPTH24_STENCIL8);

        }


        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            TRI_CORE_ERROR("Incomplete FrameBuffer!");
            TRI_CORE_ASSERT(false, "");
        }
    }
}