#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "RenderPass.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Core/Base/Core.h"
#include <glm/glm.hpp>

namespace TriEngine {

	struct TexturedQuad {
		glm::mat4 Transform;
		glm::vec4 Tint = glm::vec4(1.0f);
		Reference<Texture2D> Texture;
		float TilingFactor = 1.0f;
		int32_t EntityId = -1;
		bool Transparent = false;
	};

	struct ColoredQuad {
		glm::mat4 Transform;
		glm::vec4 Tint = glm::vec4(1.0f);
		float TilingFactor = 1.0f;
		int32_t EntityId = -1;
		bool Transparent = false;
	};

	class Renderer2D {
	public:
		static void Init();
		static void ShutDown();

		static void ClearTextures();

		static void Begin(const glm::mat4 cameraProjection, const glm::mat4& cameraTransform, const Reference<Renderpass>& renderPass);
		static void End();
		static void Flush();

		static void SubmitQuad(const TexturedQuad& quad);
		static void SubmitQuad(const ColoredQuad& quad);

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

			int32_t ObjectID;
		};

		struct BatchSettings {
			static const uint32_t MaxBatchSize = 10'000;
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

			std::vector<QuadVertex> VertexData;
			std::vector<QuadVertex>::iterator VertexDataPtr;
			std::vector<QuadVertex>::iterator TransparentVertexDataBegin;
			std::vector<QuadVertex>::iterator TransparentVertexDataPtr;

			std::vector<Reference<Texture2D>> TextureSlots;
			uint32_t TextureSlotIndex = 1;
			
			uint32_t IndexCount = 0;
		};

		struct TransparencyKey {
			bool operator() (const QuadVertex& a, const QuadVertex& b)
			{
				return a.Position.z < b.Position.z;
			}
		};

		static RenderData s_RenderData;
	};
}
