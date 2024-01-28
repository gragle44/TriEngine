#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Core/Base/Core.h"
#include <glm/glm.hpp>

namespace TriEngine {
	struct TexturedQuad {
		glm::vec2 Position;
		glm::vec2 Size;
		Reference<Texture2D> Texture;
		glm::vec4 Tint = glm::vec4(1.0f);
		float Rotation = 0.0f;
		float SortingOrder = 0.0f;
		float TilingFactor = 1.0f;

		TexturedQuad() = default;

		TexturedQuad(const glm::vec2& position, const glm::vec2& size,
			const Reference<Texture2D>& texture, const glm::vec4& tint = glm::vec4(1.0f),
			float rotation = 0.0f, float sortingOrder = 0.0f, float tilingFactor = 1.0f)
			: Position(position), Size(size), Texture(texture), Tint(tint),
			Rotation(rotation), SortingOrder(sortingOrder), TilingFactor(tilingFactor) {}
	};

	struct ColoredQuad {
		glm::vec2 Position;
		glm::vec2 Size;
		glm::vec4 Color;
		float Rotation = 0.0f;
		float SortingOrder = 0.0f;
		float TilingFactor = 1.0f;

		ColoredQuad() = default;

		ColoredQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
			float rotation = 0.0f, float sortingOrder = 0.0f, float tilingFactor = 1.0f)
			: Position(position), Size(size), Color(color), Rotation(rotation), SortingOrder(sortingOrder), TilingFactor(tilingFactor) {}
	};

	class Renderer2D {
	public:
		static void Init();
		static void ShutDown();

		static void Begin(const OrthographicCamera& camera);
		static void End();

		static void DrawQuad(const TexturedQuad& quad);
		static void DrawQuad(const ColoredQuad& quad);
	private:
		struct RenderData {
			Reference<Shader> MainShader;
			Reference<VertexArray> VertexArray;
			Reference<Texture> DefaultTexture;
		};
		static RenderData* s_RenderData;
	};
}
