#pragma once

#include "RendererAPI.h"

namespace TriEngine {
	class RenderCommand {
	public:
		static void DrawElements(const std::shared_ptr<VertexArray>& vertexArray) {
			s_RendererAPI->DrawElements(vertexArray);
		}

		static void Clear() {
			s_RendererAPI->Clear();
		}

		static void ClearDepth() {
			s_RendererAPI->ClearDepth();
		}

		static void ClearColor(const glm::vec4& color) {
			s_RendererAPI->ClearColor(color);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}