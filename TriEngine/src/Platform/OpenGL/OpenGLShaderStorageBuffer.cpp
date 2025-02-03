#include "tripch.h"
#include "OpenGLShaderStorageBuffer.h"

#include <glad/glad.h>

namespace TriEngine {
	OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const void* data, uint32_t size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferStorage(m_BufferID, size, data, GL_DYNAMIC_STORAGE_BIT);
	}

	OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OpenGLShaderStorageBuffer::Bind(uint32_t slot) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, m_BufferID);
	}

	void OpenGLShaderStorageBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_BufferID, offset, size, data);
	}
}