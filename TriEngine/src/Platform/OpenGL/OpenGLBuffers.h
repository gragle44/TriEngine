#pragma once

#include "Renderer/Buffers.h"

namespace TriEngine {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void BindToVertexArray(uint32_t arrayID) const final;

		virtual uint32_t GetID() const final { return m_BufferID; }

		virtual void SetData(const void* data, uint32_t size) final;

		virtual void SetLayout(const BufferLayout& layout) final { m_Layout = layout; };
		virtual const BufferLayout& GetLayout() const final { return m_Layout; };
	private:
		BufferLayout m_Layout;
		uint32_t m_BufferID;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void BindToVertexArray(uint32_t arrayID) const final;

		virtual uint32_t GetID() const final { return m_BufferID; }

		virtual uint32_t GetCount() const final { return m_Count; }
	private:
		uint32_t m_Count;
		uint32_t m_BufferID;
	};
}