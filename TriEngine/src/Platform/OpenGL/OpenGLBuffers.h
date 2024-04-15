#pragma once

#include "Renderer/Buffers.h"

namespace TriEngine {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void BindToVertexArray(uint32_t arrayID) const final;

		virtual RID GetID() const final { return m_BufferID; }

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) final;

		virtual void SetLayout(const BufferLayout& layout) final { m_Layout = layout; };
		virtual const BufferLayout& GetLayout() const final { return m_Layout; };
		virtual const uint32_t GetVertexCount() const final { return m_Size / m_Layout.GetStride(); }
		virtual const uint32_t GetSize() const final { return m_Size; }
	private:
		uint32_t m_Size;
		BufferLayout m_Layout;
		RID m_BufferID;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void BindToVertexArray(uint32_t arrayID) const final;

		virtual RID GetID() const final { return m_BufferID; }

		virtual uint32_t GetCount() const final { return m_Count; }
	private:
		uint32_t m_Count;

		RID m_BufferID;
	};
}