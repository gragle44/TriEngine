#pragma once

#include "RendererAPI.h"

namespace TriEngine {
	class RenderCommand {
	public:
		static void Init() {
			s_RendererAPI->Init();
		}

		static void DrawElements(const Reference<VertexArray>& vertexArray, uint32_t count = 0) {
			s_RendererAPI->DrawElements(vertexArray, count);
		}

		static void DrawArrays(const Reference<VertexArray>& vertexArray, uint32_t vertexBufferIndex = 0, uint32_t vertexCount = 0) {
			s_RendererAPI->DrawArrays(vertexArray, vertexBufferIndex, vertexCount);
		}

		static void SetViewPort(const glm::ivec4& dimensions) {
			s_RendererAPI->SetViewPort(dimensions);
		}

		static void DepthTest(bool enable) {
			s_RendererAPI->DepthTest(enable);
		}

		static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->ClearColor(color);
		}

		static void Clear() {
			s_RendererAPI->Clear();
		}

		static void EnableWireframes(bool enabled) {
			s_RendererAPI->EnableWireframes(enabled);
		}

		static uint32_t GetMaxTextureSlots() {
			return s_RendererAPI->GetMaxTextureSlots();
		}

	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};
}