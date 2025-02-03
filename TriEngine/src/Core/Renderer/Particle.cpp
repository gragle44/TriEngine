#include "tripch.h"

#include "Particle.h"

#include "RenderCommand.h"

#include "Core/Base/Random.h"

namespace TriEngine {

	struct GPUParticleEmmiter {
		glm::vec4 MinColor, MaxColor;
		glm::vec3 MinOffset, MaxOffset;
		glm::vec2 MinVelocity, MaxVelocity;
		glm::vec2 MinAcceleration, MaxAcceleration;
		float MinLifetime, MaxLifetime;

		glm::vec3 Position;

		float SpawnInterval, SpawnTimer;

		int32_t Texture;
	};

	ParticleSystemData ParticleSystem::m_Data = ParticleSystemData();

	void ParticleSystem::Init()
	{

		static const glm::vec2 baseQuadPositions[4] = { { -0.5f, -0.5f }, { 0.5f, -0.5f }, { 0.5f,  0.5f }, { -0.5f,  0.5f } };

		std::vector<Particle> particleBufferData(m_Data.MaxParticles);
		std::unique_ptr<ParticleFreeList> freelistBufferData(std::make_unique<ParticleFreeList>());
		freelistBufferData->Count = m_Data.MaxParticles;

		for (int i = 0; i < m_Data.MaxParticles; i++) {
			freelistBufferData->AvailiableIndices[i] = i;
		}

		m_Data.ParticleBuffer = ShaderStorageBuffer::Create(particleBufferData.data(), sizeof(Particle) * m_Data.MaxParticles);
		m_Data.FreelistBuffer = ShaderStorageBuffer::Create(freelistBufferData.get(), sizeof(ParticleFreeList));

		m_Data.DefaultTexture = Texture2D::Create(glm::vec4(1.0f), 1);
		m_Data.TextureSlots.resize(32);
		m_Data.TextureSlots[0] = m_Data.DefaultTexture;

		m_Data.RenderShader = Shader::Create("ParticleRenderShader", "src/shaders/particle_vs.glsl", "src/shaders/particle_fs.glsl");
		m_Data.UpdateShader = ComputeShader::Create("ParticleUpdateShader", "src/shaders/particleUpdate_cs.glsl");
		m_Data.EmmisionShader = ComputeShader::Create("ParticleEmmisionShader", "src/shaders/particleSpawner_cs.glsl");

		int32_t* samplers = new int32_t[32];
		for (int32_t i = 0; i < 32; i++)
			samplers[i] = i;

		m_Data.RenderShader->Bind();
		m_Data.RenderShader->SetIntArray("u_Samplers", samplers, 32);

		delete[] samplers;

		m_Data.VertexArray = VertexArray::Create();

		m_Data.VertexBuffer = VertexBuffer::Create((float*)& baseQuadPositions, sizeof(baseQuadPositions));

		{
			TriEngine::BufferLayout layout = {
				{ "a_Position", TriEngine::ShaderDataType::Float2 }
			};

			m_Data.VertexBuffer->SetLayout(layout);
		}

		uint32_t quadIndices[] = {0, 1, 2, 2, 3, 0};


		Reference<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, 6);

		m_Data.VertexArray->AddVertexAndIndexBuffers(m_Data.VertexBuffer, indexBuffer);
	}

	void ParticleSystem::Update(float deltaTime)
	{
		m_Data.ShouldRender = true;
		m_Data.UpdateShader->SetFloat("u_DeltaTime", deltaTime);

		int workGroupSize = (m_Data.MaxParticles + 64 - 1) / 64;
		m_Data.UpdateShader->Dispatch(workGroupSize, 1, 1);
	}

	void ParticleSystem::UpdateEmmitter(float deltaTime, ParticleEmmiterComponent& emmiter, Transform2DComponent& transform)
	{
		if (emmiter.SpawnInterval == 0.0) {
			TRI_CORE_WARN("Cannot update a particle emmitter with a spawn interval of 0");
			return;
		}
		emmiter.SpawnTimer += deltaTime;

		int particlesToSpawn = emmiter.SpawnTimer / emmiter.SpawnInterval;

		emmiter.SpawnTimer -= emmiter.SpawnInterval * particlesToSpawn;

		if (particlesToSpawn == 0)
			return;
		
		uint32_t texIndex = 0;

		if (emmiter.Texture) {
			for (uint32_t i = 1; i < m_Data.TextureSlotIndex; i++) {
				if (m_Data.TextureSlots[i]->GetID() == emmiter.Texture->GetID()) {
					texIndex = i;
					break;
				}
			}

			if (texIndex == 0) {
				if (m_Data.TextureSlotIndex >= 32) {
					TRI_CORE_WARN("There is a maximum of 32 particle emmitters");
					return;
				}

				else {
					m_Data.TextureSlots[m_Data.TextureSlotIndex] = emmiter.Texture;
					texIndex = m_Data.TextureSlotIndex;
					m_Data.TextureSlotIndex++;
				}
			}
		}

		//unforms
		GPUParticleEmmiter emmitterUniform = {
			emmiter.MinColor, emmiter.MaxColor,
			emmiter.MinOffset, emmiter.MaxOffset,
			emmiter.MinVelocity, emmiter.MaxVelocity,
			emmiter.MinAccel, emmiter.MaxAccel,
			emmiter.MinLife, emmiter.MaxLife,
			transform.Position,
			emmiter.SpawnInterval, emmiter.SpawnTimer,
			texIndex
		};

		m_Data.EmmisionShader->SetInt("u_ParticlesToSpawn", particlesToSpawn);

		m_Data.EmmisionShader->SetInt("u_Seed", Random::Int(0, INT16_MAX));

		//TODO: this is weird, fix it later
		m_Data.EmmisionShader->SetFloat4("u_Emitter.MinColor", emmitterUniform.MinColor);
		m_Data.EmmisionShader->SetFloat4("u_Emitter.MaxColor", emmitterUniform.MaxColor);
		m_Data.EmmisionShader->SetFloat3("u_Emitter.MinOffset", emmitterUniform.MinOffset);
		m_Data.EmmisionShader->SetFloat3("u_Emitter.MaxOffset", emmitterUniform.MaxOffset);
		m_Data.EmmisionShader->SetFloat2("u_Emitter.MinVelocity", emmitterUniform.MinVelocity);
		m_Data.EmmisionShader->SetFloat2("u_Emitter.MaxVelocity", emmitterUniform.MaxVelocity);
		m_Data.EmmisionShader->SetFloat2("u_Emitter.MinAcceleration", emmitterUniform.MinAcceleration);
		m_Data.EmmisionShader->SetFloat2("u_Emitter.MaxAcceleration", emmitterUniform.MaxAcceleration);
		m_Data.EmmisionShader->SetFloat("u_Emitter.MinLifetime", emmitterUniform.MinLifetime);
		m_Data.EmmisionShader->SetFloat("u_Emitter.MaxLifetime", emmitterUniform.MaxLifetime);
		m_Data.EmmisionShader->SetFloat3("u_Emitter.Position", transform.Position);
		m_Data.EmmisionShader->SetFloat("u_Emitter.SpawnInterval", emmitterUniform.SpawnInterval);
		m_Data.EmmisionShader->SetFloat("u_Emitter.SpawnTimer", emmitterUniform.SpawnTimer);
		m_Data.EmmisionShader->SetInt("u_Emitter.TextureIndex", emmitterUniform.Texture);


		int workGroupSize = (particlesToSpawn + 64 - 1) / 64;
		m_Data.EmmisionShader->Dispatch(workGroupSize, 1, 1);
	}

	void ParticleSystem::Render(const glm::mat4& viewProj)
	{
		if (!m_Data.ShouldRender)
			return;
		m_Data.RenderShader->Bind();
		m_Data.RenderShader->SetMat4("u_ViewProj", viewProj);

		for (uint32_t i = 0; i < m_Data.TextureSlotIndex; i++) {
			m_Data.TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawElementsInstanced(m_Data.VertexArray, 6, m_Data.MaxParticles);

		m_Data.ShouldRender = false;
	}

	ParticleSystemData& ParticleSystem::GetData()
	{
		return m_Data;
	}
}
