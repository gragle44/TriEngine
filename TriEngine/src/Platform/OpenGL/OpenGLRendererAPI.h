#pragma once

#include "Renderer/RendererAPI.h"

namespace TriEngine {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init() final;
		virtual void Shutdown() final;

		virtual void EnableWireframes(bool enabled) const final;

		virtual void SetLineWidth(uint32_t width) const final;

		virtual void DepthTest(bool enable) const final;

		virtual void ClearColor(const glm::vec4& color) const final;

		virtual void Clear() const final;

		virtual void MemoryBarrier() const final;

		virtual void SetViewPort(const glm::ivec4& dimensions) const final;

		virtual void DrawElements(const Reference<VertexArray>& vertexArray, uint32_t count = 0) const final;
		virtual void DrawElementsInstanced(const Reference<VertexArray>& vertexArray, uint32_t elementCount, uint32_t instanceCount) const final;
		virtual void DrawArrays(const Reference<VertexArray>& vertexArray, uint32_t vertexBufferIndex = 0, uint32_t vertexCount = 0) const final;
		virtual void DrawLines(const Reference<VertexArray>& vertexArray, uint32_t vertexBufferIndex = 0, uint32_t vertexCount = 0) const final;

		virtual uint32_t GetMaxTextureSlots() const final;
		virtual RendererCapabilities& GetRendererCaps() final;
	};
}