#pragma once

#include "Renderer/Buffers.h"

namespace TriEngine {
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual uint32_t GetID() const final { return m_BufferID; };

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

		virtual void Bind() const final;
		virtual void Unbind() const final;

		virtual uint32_t GetID() const final { return m_BufferID; };

		virtual inline uint32_t GetCount() const final { return m_Count; }
	private:
		uint32_t m_Count;
		uint32_t m_BufferID;
	};
}