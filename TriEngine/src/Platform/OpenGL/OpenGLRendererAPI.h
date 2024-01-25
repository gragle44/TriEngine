#pragma once

#include "Renderer/RendererAPI.h"

namespace TriEngine {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void Init() final;

		virtual void EnableWireframes(bool enabled) const final;

		virtual void ClearColor(const glm::vec4& color) const final;
		virtual void ClearDepth() const final;

		virtual void Clear() const final;

		virtual void SetViewPort(const glm::ivec4& dimensions) const final;

		virtual void DrawElements(const Reference<VertexArray>& vertexArray) const final;

	};
}