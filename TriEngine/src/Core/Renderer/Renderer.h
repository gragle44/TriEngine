#pragma once

#include "RenderCommand.h"

namespace TriEngine {
	class Renderer {
	public:
		static void Begin();
		static void End();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetApi(); }
	};
}