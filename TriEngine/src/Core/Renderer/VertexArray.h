#pragma once

#include "Buffers.h"
#include <memory>

namespace TriEngine {
	class VertexArray {
	public:
		virtual ~VertexArray() {}

		virtual void AddVertexAndIndexBuffers(const Reference<VertexBuffer>& vertexBuffer, const Reference<IndexBuffer>&) = 0;

		virtual void AddVertexBuffer(const Reference<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Reference<IndexBuffer>& indexBuffer) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::vector<Reference<VertexBuffer>> GetVertexBuffers() const = 0;
		virtual const Reference<IndexBuffer> GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}
