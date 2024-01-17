#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace TriEngine {
	class Renderer {
	public:
		static void Begin(const OrthographicCamera& camera);
		static void End();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

		static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetApi(); }
	private:
		static glm::mat4 s_ViewProjectionMatrix;
	};
}