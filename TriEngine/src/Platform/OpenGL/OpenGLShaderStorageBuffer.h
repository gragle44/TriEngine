#pragma once

#include "Core/Renderer/ShaderStorageBuffer.h"

namespace TriEngine {
	class OpenGLShaderStorageBuffer : public ShaderStorageBuffer {
	public:
		OpenGLShaderStorageBuffer(const void* data, uint32_t size);
		virtual ~OpenGLShaderStorageBuffer() override;

		virtual void Bind(uint32_t slot) const override;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;
	private:
		uint32_t m_BufferID;
	};
}
