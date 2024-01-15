#pragma once

#include "Buffers.h"
#include <memory>

namespace TriEngine {
	class VertexArray {
	public:
		virtual ~VertexArray() {}

		virtual void AddVertexAndIndexBuffers(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>&) = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<IndexBuffer> GetIndexBuffers() const = 0;

		static VertexArray* Create();
	};
}
