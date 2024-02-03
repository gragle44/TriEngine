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
		glm::ivec2 AtlasIndex = glm::ivec2(-1);
		glm::vec4 Tint = glm::vec4(1.0f);
		float Rotation = 0.0f;
		float SortingOrder = 0.0f;
		float TilingFactor = 1.0f;

		TexturedQuad() = default;

		TexturedQuad(const glm::vec2& position, const glm::vec2& size,
			const Reference<Texture2D>& texture, const glm::vec2& atlasIndex = glm::ivec2(-1), const glm::vec4& tint = glm::vec4(1.0f),
			float rotation = 0.0f, float sortingOrder = 0.0f, float tilingFactor = 1.0f)
			: Position(position), Size(size), Texture(texture), AtlasIndex(atlasIndex), Tint(tint),
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
		static void Flush();

		static void SubmitQuad(const ColoredQuad& quad);
		static void SubmitQuad(const TexturedQuad& quad);

		struct RenderStats {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t VertexCount() { return QuadCount * 4; }
			uint32_t IndexCount() { return QuadCount * 6; }

			void Reset() {
				memset(this, 0, sizeof(RenderStats));
			}
		};

		static RenderStats GetStats();
	private:
		static void NewBatch();

	private:
		struct QuadVertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;

			uint32_t TexIndex;
			float TilingFactor;
		};

		struct BatchSettings {
			static const uint32_t MaxBatchSize = 15'000;
			static const uint32_t MaxVertices = MaxBatchSize * 4;
			static const uint32_t MaxIndices = MaxBatchSize * 6;
			static uint32_t MaxTextureSlots;
		};

		struct RenderData {
			RenderStats Stats;
			
			Reference<Shader> MainShader;
			Reference<VertexArray> VertexArray;
			Reference<VertexBuffer> VertexBuffer;
			Reference<Texture2D> DefaultTexture;

			std::vector<QuadVertex> VertexData;
			std::vector<QuadVertex>::iterator VertexDataPtr;
			std::vector<Reference<Texture2D>> TextureSlots;
			uint32_t TextureSlotIndex = 1;
			
			uint32_t IndexCount = 0;
		};

		static RenderData* s_RenderData;
	};
}
