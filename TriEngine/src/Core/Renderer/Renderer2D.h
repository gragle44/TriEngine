#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Core/Base/Core.h"
#include <glm/glm.hpp>

namespace TriEngine {
	struct Quad {
		glm::vec2 Position;
		glm::vec2 Size;
		Reference<Texture2D> Texture;
	};

	class Renderer2D {
	public:
		static void Init();

		static void Begin(const OrthographicCamera& camera);
		static void End();

		static void DrawQuad(const Quad& quad);
	private:
		struct RenderData {
			Reference<Shader> MainShader;
			Reference<VertexArray> QuadVertexArray;
		};
		static RenderData* s_RenderData;
	};
}
