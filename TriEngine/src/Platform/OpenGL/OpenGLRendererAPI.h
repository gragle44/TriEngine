#pragma once

#include "Renderer/RendererAPI.h"

namespace TriEngine {
	class OpenGLRendererAPI : public RendererAPI {
	public:
		virtual void ClearColor(const glm::vec4& color) const final;
		virtual void ClearDepth() const final;

		virtual void Clear() const final;

		virtual void DrawElements(const std::shared_ptr<VertexArray>& vertexArray) const final;

	};
}