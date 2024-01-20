#pragma once

#include "RendererAPI.h"

namespace TriEngine {
	class RenderCommand {
	public:
		static void DrawElements(const Reference<VertexArray>& vertexArray) {
			s_RendererAPI->DrawElements(vertexArray);
		}

		static void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewPort(x, y, width, height);
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