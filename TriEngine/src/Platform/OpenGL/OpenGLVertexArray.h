#pragma once

#include "Renderer/VertexArray.h"

namespace TriEngine {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void AddVertexAndIndexBuffers(const Reference<VertexBuffer>& vertexBuffer, const Reference<IndexBuffer>&) final;

		virtual void AddVertexBuffer(const Reference<VertexBuffer>& vertexBuffer) final;
		virtual void SetIndexBuffer(const Reference<IndexBuffer>&) final;

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual const std::vector<Reference<VertexBuffer>> GetVertexBuffers() const final { return m_VertexBuffers; };
		virtual const Reference<IndexBuffer> GetIndexBuffer() const final { return m_IndexBuffer; }
	private:
		std::vector<Reference<VertexBuffer>> m_VertexBuffers;
		Reference<IndexBuffer> m_IndexBuffer;

		uint32_t m_VertexBufferIndexOffset = 0;
		RID m_ArrayID;
	};
}
