#include "tripch.h"
#include "OpenGLVertexArray.h"

#include "Base/Assert.h"

#include <Glad/glad.h>

namespace TriEngine {

	static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		TRI_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_ArrayID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_ArrayID);
	}

	void OpenGLVertexArray::AddVertexAndIndexBuffers(const Reference<VertexBuffer>& vertexBuffer, const Reference<IndexBuffer>& indexBuffer)
	{
		AddVertexBuffer(vertexBuffer);
		SetIndexBuffer(indexBuffer);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Reference<VertexBuffer>& vertexBuffer)
	{
		vertexBuffer->BindToVertexArray(m_ArrayID);

		TRI_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		uint32_t index = 0;
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout)
		{
			glEnableVertexArrayAttrib(m_ArrayID, index + m_VertexBufferIndexOffset);

			if (element.DataType == ShaderDataType::Int)
				glVertexArrayAttribIFormat(
					m_ArrayID,
					index,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLType(element.DataType),
					element.Offset
				);
			else 
				glVertexArrayAttribFormat(m_ArrayID,
					index,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLType(element.DataType),
					element.Normalized ? GL_TRUE : GL_FALSE,
					element.Offset);
			glVertexArrayAttribBinding(m_ArrayID, index + m_VertexBufferIndexOffset, 0);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
		m_VertexBufferIndexOffset += layout.GetElements().size();
	}

	void OpenGLVertexArray::SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer)
	{
		indexBuffer->BindToVertexArray(m_ArrayID);
		m_IndexBuffer = indexBuffer;
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_ArrayID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}
}