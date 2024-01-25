#pragma once

#include "RendererAPI.h"

namespace TriEngine {
	class RenderCommand {
	public:
		static void Init() {
			s_RendererAPI->Init();
		}

		static void DrawElements(const Reference<VertexArray>& vertexArray) {
			s_RendererAPI->DrawElements(vertexArray);
		}

		static void SetViewPort(const glm::ivec4& dimensions) {
			s_RendererAPI->SetViewPort(dimensions);
		}

		static void Clear() {
			s_RendererAPI->Clear();
		}

		static void EnableWireframes(bool enabled) {
			s_RendererAPI->EnableWireframes(enabled);
		}

		static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->ClearColor(color);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};
}