#pragma once

#include "Renderer/VertexArray.h"

namespace TriEngine {
	class OpenGLVertexArray : public VertexArray {
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void AddVertexAndIndexBuffers(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>&) final;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) final;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>&) final;

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual const std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const final { return m_VertexBuffers; };
		virtual const std::shared_ptr<IndexBuffer> GetIndexBuffer() const final { return m_IndexBuffer; }
	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		uint32_t m_ArrayID;
	};
}
