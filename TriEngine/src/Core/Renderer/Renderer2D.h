#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "RenderPass.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Core/Base/Core.h"

namespace TriEngine {
	struct TexturedQuad {
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Size = glm::vec2(1.0f);
		Reference<Texture2D> Texture;
		glm::vec4 Tint = glm::vec4(1.0f);
		float Rotation = 0.0f;
		float SortingOrder = 0.0f;
		float TilingFactor = 1.0f;
	};

	struct TexturedQuadn {
		glm::mat4 Transform;
		Reference<Texture2D> Texture;
		float TilingFactor = 1.0f;
		glm::vec4 Tint = glm::vec4(1.0f);
	};

	struct SubTexturedQuad {
		glm::vec2 Position = glm::vec2(0.0f);
		glm::vec2 Size = glm::vec2(1.0f);
		Reference<SubTexture2D> Texture;
		glm::ivec2 AtlasIndex = glm::ivec2(0);
		glm::vec4 Tint = glm::vec4(1.0f);
		float Rotation = 0.0f;
		float SortingOrder = 0.0f;
		float TilingFactor = 1.0f;
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

		static void Begin(const OrthographicCameraOld& camera, const Reference<FrameBuffer>& frameBuffer);
		static void Begin(const glm::mat4 cameraProjection, const glm::mat4& cameraTransform, const Reference<Renderpass>& renderPass);
		static void End();
		static void Flush();

		static void SubmitQuad(const ColoredQuad& quad);
		static void SubmitQuad(const TexturedQuad& quad);
		static void SubmitQuad(const TexturedQuadn& quad);
		static void SubmitQuad(const SubTexturedQuad& quad);

		struct RenderStats {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t VertexCount() const { return QuadCount * 4; }
			uint32_t IndexCount() const { return QuadCount * 6; }

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
			Reference<VertexArray> QuadVertexArray;
			Reference<VertexBuffer> QuadVertexBuffer;
			Reference<Texture2D> DefaultTexture;

			Reference<Renderpass> CurrentPass;
			Reference<FrameBuffer> ScreenFrameBuffer;
			Reference<VertexArray> ScreenVertexArray;
			Reference<VertexBuffer> ScreenVertexBuffer;
			Reference<Shader> ScreenShader;

			std::vector<QuadVertex> VertexData;
			std::vector<QuadVertex>::iterator VertexDataPtr;

			std::vector<Reference<Texture2D>> TextureSlots;
			uint32_t TextureSlotIndex = 1;
			
			uint32_t IndexCount = 0;
		};

		static RenderData s_RenderData;
	};
}
