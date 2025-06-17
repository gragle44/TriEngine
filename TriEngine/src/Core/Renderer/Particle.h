#pragma once

#include "glm/glm.hpp"
#include "ShaderStorageBuffer.h"
#include "Buffers.h"
#include "VertexArray.h"
#include "Shader.h"

#include "Core/GameObjects/Components.h"


namespace TriEngine {

	#define MAX_PARTICLES 100000

	struct ParticleSystemData {
		Reference<ShaderStorageBuffer> ParticleBuffer;
		Reference<ShaderStorageBuffer> FreelistBuffer;

		Reference<ComputeShader> UpdateShader;
		Reference<ComputeShader> EmmisionShader;
		Reference<Shader> RenderShader;

		Reference<Texture2D> DefaultTexture;

		Reference<VertexArray> vertexArray;
		Reference<VertexBuffer> vertexBuffer;
		Reference<IndexBuffer> indexBuffer;

		bool ShouldRender = false;

		std::vector<Reference<Texture2D>> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		const uint32_t MaxParticles = MAX_PARTICLES;
	};

	class ParticleSystem {
	public:
		ParticleSystem() = default;
		~ParticleSystem() = default;

		static void Init();

		static void Update(float deltaTime);
		
		static void UpdateEmmitter(float deltaTime, ParticleEmmiterComponent& emmiter, Transform2DComponent& transform);

		static void Render(const glm::mat4& viewProj);

		static ParticleSystemData& GetData();
	private:
		static ParticleSystemData m_Data;
	};

	struct alignas(16) Particle {
		glm::vec3 Position;
		float padding1;
		glm::vec2 Velocity;
		glm::vec2 Acceleration;
		glm::vec4 Color;
		float Lifetime;

		int TextureIndex;
	};

	struct ParticleFreeList {
		int32_t Count;
		int32_t AvailiableIndices[MAX_PARTICLES];
	};
}
