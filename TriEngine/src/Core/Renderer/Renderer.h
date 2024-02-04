#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace TriEngine {
	class Renderer {
	public:
		static void Init();

		static RendererAPI::API GetCurrentAPI() { return RendererAPI::GetApi(); }
	private:
		static glm::mat4 s_ViewProjectionMatrix;
	};
}