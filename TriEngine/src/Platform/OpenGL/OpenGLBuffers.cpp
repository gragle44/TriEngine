#include "tripch.h"

#include "OpenGLBuffers.h"

#include <Glad/glad.h>

namespace TriEngine {
	//Vertex Buffer
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
		:m_Size(size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferStorage(m_BufferID, size , vertices, GL_DYNAMIC_STORAGE_BIT);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
		:m_Size(size)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferStorage(m_BufferID, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OpenGLVertexBuffer::BindToVertexArray(uint32_t arrayID) const
	{
		glVertexArrayVertexBuffer(arrayID, 0, m_BufferID, 0, m_Layout.GetStride());
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glNamedBufferSubData(m_BufferID, 0, size, data);
	}
	
	//Index Buffer

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		glCreateBuffers(1, &m_BufferID);
		glNamedBufferStorage(m_BufferID, count * sizeof(uint32_t), indices, GL_DYNAMIC_STORAGE_BIT);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_BufferID);
	}

	void OpenGLIndexBuffer::BindToVertexArray(uint32_t arrayID) const
	{
		glVertexArrayElementBuffer(arrayID, m_BufferID);
	}

}