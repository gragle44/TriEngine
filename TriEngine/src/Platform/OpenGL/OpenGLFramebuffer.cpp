#include "tripch.h"
#include "OpenGLFramebuffer.h"
#include "Base/Assert.h"
#include "Base/Application.h"
#include "Core/Renderer/RenderCommand.h"

#include <glad/glad.h>

namespace TriEngine {
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSettings& settings)
        :m_Settings(settings)
    {
        Recreate();
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        //Texture atachments will be deleted automatically
        glDeleteFramebuffers(1, &m_BufferID);
        glDeleteRenderbuffers(1, &m_RenderBuffer);
    }

    void OpenGLFrameBuffer::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferID);
        glViewport(0, 0, m_Settings.Width, m_Settings.Height);
    }

    void OpenGLFrameBuffer::UnBind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::ReSize(uint32_t width, uint32_t height)
    {
        //TODO: Move to RenderCommand
        int32_t maxSize = RenderCommand::GetCapabilities().MaxFramebufferSize;

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

    void OpenGLFrameBuffer::BindColorAttachment()
    {
        m_ColorTarget->Bind(0);
    }

    void OpenGLFrameBuffer::Recreate()
    {
        if (m_BufferID) {
            glDeleteFramebuffers(1, &m_BufferID);
                
            m_ColorTarget.reset();

            glDeleteRenderbuffers(1, &m_RenderBuffer);
        }

        glCreateFramebuffers(1, &m_BufferID);

        m_ColorTarget = Texture2D::Create(glm::ivec2(m_Settings.Width, m_Settings.Height), {.Filter = TextureFilter::Linear, .Wrap = TextureWrap::ClampEdge, .Samples = m_Settings.Samples});
        glNamedFramebufferTexture(m_BufferID, GL_COLOR_ATTACHMENT0, m_ColorTarget->GetID(), 0);

        glCreateRenderbuffers(1, &m_RenderBuffer);
        if (m_Settings.Samples > 1) {
            glNamedRenderbufferStorageMultisample(m_RenderBuffer, m_Settings.Samples, GL_DEPTH24_STENCIL8, m_Settings.Width, m_Settings.Height);
        }
        else {
            glNamedRenderbufferStorage(m_RenderBuffer, GL_DEPTH24_STENCIL8, m_Settings.Width, m_Settings.Height);
        }
        glNamedFramebufferRenderbuffer(m_BufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            TRI_CORE_ERROR("Incomplete FrameBuffer!");
            TRI_CORE_ASSERT(false, "");
        }
    }
}