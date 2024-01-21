#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace TriEngine {
	class Renderer {
	public:
		static void Init();

		static void Begin(const OrthographicCamera& camera);
		static void End();

		static void Submit(const Reference<Shader>& shader, const Reference<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetApi(); }
	private:
		static glm::mat4 s_ViewProjectionMatrix;
	};
}