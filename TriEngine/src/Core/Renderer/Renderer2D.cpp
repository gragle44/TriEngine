#include "tripch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	Renderer2D::RenderData Renderer2D::s_RenderData = RenderData();
	uint32_t Renderer2D::BatchSettings::MaxTextureSlots;

	static constexpr glm::vec4 baseQuadPosition[4] = { { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f,  0.5f, 0.0f, 1.0f }, { -0.5f,  0.5f, 0.0f, 1.0f }};
	static constexpr glm::vec2 baseTexCoord[4] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

	static float fullscreenQuadVertices[] = {
		// positions         // texCoords
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
	};
	

	void Renderer2D::Init()
	{
		BatchSettings::MaxTextureSlots = RenderCommand::GetCapabilities().MaxSamplers;
		s_RenderData.TextureSlots.resize(BatchSettings::MaxTextureSlots);
					
		s_RenderData.ScreenVertexArray = VertexArray::Create();
		s_RenderData.ScreenVertexBuffer = VertexBuffer::Create(fullscreenQuadVertices, sizeof(fullscreenQuadVertices));

		{
			TriEngine::BufferLayout layout = {
				{ "a_Position", TriEngine::ShaderDataType::Float3 },
				{ "a_TexCoord", TriEngine::ShaderDataType::Float2 },
			};

			s_RenderData.ScreenVertexBuffer->SetLayout(layout);
		}

		s_RenderData.ScreenVertexArray->AddVertexBuffer(s_RenderData.ScreenVertexBuffer);

		s_RenderData.ScreenShader = Shader::Create("ScreenShader", "src/Shaders/screenvert.glsl", "src/Shaders/screenfrag.glsl");
		s_RenderData.ScreenShader->SetInt("u_ScreenSampler", 0);
		
		s_RenderData.QuadVertexArray = VertexArray::Create();
		
		s_RenderData.VertexData.resize(BatchSettings::MaxVertices);
		s_RenderData.VertexDataPtr = s_RenderData.VertexData.begin();
					
		s_RenderData.QuadVertexBuffer = VertexBuffer::Create(BatchSettings::MaxVertices * sizeof(QuadVertex));

		{
			TriEngine::BufferLayout layout = {
				{ "a_Position", TriEngine::ShaderDataType::Float3 },
				{ "a_Color", TriEngine::ShaderDataType::Float4 },
				{ "a_TexCoord", TriEngine::ShaderDataType::Float2 },
				{ "a_TexIndex", TriEngine::ShaderDataType::Int},
			};

			s_RenderData.QuadVertexBuffer->SetLayout(layout);
		}	

		uint32_t* quadIndices = new uint32_t[BatchSettings::MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < BatchSettings::MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Reference<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, BatchSettings::MaxIndices);

		s_RenderData.QuadVertexArray->AddVertexAndIndexBuffers(s_RenderData.QuadVertexBuffer, indexBuffer);

		delete[] quadIndices;

		int32_t* samplers = (int32_t*)alloca(BatchSettings::MaxTextureSlots * sizeof(int32_t));

		for (int32_t i = 0; i < (int32_t)BatchSettings::MaxTextureSlots; i++)
			samplers[i] = i;


		s_RenderData.MainShader = Shader::Create("TextureShader", "src/Shaders/basicvert.glsl", "src/Shaders/basicfrag.glsl");

		s_RenderData.MainShader->SetIntArray("u_Samplers", samplers, BatchSettings::MaxTextureSlots);

		s_RenderData.DefaultTexture = Texture2D::Create(glm::vec4(1.0f), 1);
		s_RenderData.TextureSlots[0] = s_RenderData.DefaultTexture;
	}

	void Renderer2D::ShutDown()
	{
	}

	void Renderer2D::Begin(const OrthographicCameraOld& camera, const Reference<FrameBuffer>& frameBuffer)
	{
		if (!s_RenderData.ScreenFrameBuffer)
			s_RenderData.ScreenFrameBuffer = frameBuffer;

		frameBuffer->Bind();
		RenderCommand::Clear();
		RenderCommand::DepthTest(true);

		s_RenderData.Stats.Reset();
		s_RenderData.MainShader->Bind();
		s_RenderData.MainShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		NewBatch();
	}

	void Renderer2D::Begin(const glm::mat4 cameraProjection, const glm::mat4& cameraTransform, const Reference<Renderpass>& renderPass)
	{
		s_RenderData.CurrentPass = renderPass;

		if (renderPass->Target)
			renderPass->Target->Bind();

		if (renderPass->Clear)
			RenderCommand::Clear();
		RenderCommand::DepthTest(renderPass->DepthTest);

		s_RenderData.Stats.Reset();

		glm::mat4 viewProj = cameraProjection * glm::inverse(cameraTransform);
		s_RenderData.MainShader->Bind();
		s_RenderData.MainShader->SetMat4("u_ViewProjection", viewProj);

		NewBatch();
	}

	void Renderer2D::End()
	{
		Flush();

		if (s_RenderData.CurrentPass->Target != nullptr) {
			s_RenderData.CurrentPass->Target->UnBind();
			RenderCommand::Clear();
			RenderCommand::DepthTest(false);

			s_RenderData.ScreenShader->Bind();
			s_RenderData.CurrentPass->Target->BindColorAttachment();

			RenderCommand::DrawArrays(s_RenderData.ScreenVertexArray);
		}
		else {
			RenderCommand::Clear();
			RenderCommand::DepthTest(false);
		}
	}

	void Renderer2D::Flush()
	{
		if (s_RenderData.IndexCount == 0) {
			TRI_CORE_WARN("Attempted to draw without valid quads");
			return;
		}

		//Update Vertex Buffer
		uint32_t size = (uint32_t)std::distance(s_RenderData.VertexData.begin(), s_RenderData.VertexDataPtr);
		s_RenderData.QuadVertexBuffer->SetData(s_RenderData.VertexData.data(), size * sizeof(QuadVertex));

		//Update bound textures
		for (uint32_t i = 0; i < s_RenderData.TextureSlotIndex; i++) {
			s_RenderData.TextureSlots[i]->Bind(i);
		}

		//Draw
		RenderCommand::DrawElements(s_RenderData.QuadVertexArray, s_RenderData.IndexCount);
		s_RenderData.Stats.DrawCalls++;
	}

	void Renderer2D::NewBatch()
	{
		s_RenderData.Stats.QuadCount += s_RenderData.IndexCount / 6;

		s_RenderData.VertexDataPtr = s_RenderData.VertexData.begin();
		s_RenderData.IndexCount = 0;

		s_RenderData.TextureSlotIndex = 1;
	}

	void Renderer2D::SubmitQuad(const ColoredQuad& quad)
	{
		if (s_RenderData.IndexCount >= BatchSettings::MaxIndices) {
			Flush();
			NewBatch();
		}

		glm::mat4 rotation(1.0f);

		if (quad.Rotation != 0.0f) {
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.Rotation), { 0.0f, 0.0f, 1.0f });
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), { quad.Position.x, quad.Position.y, quad.SortingOrder }) *
			rotation *
			glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_RenderData.VertexDataPtr->Position = transform * baseQuadPosition[i];
			s_RenderData.VertexDataPtr->Color = quad.Color;
			s_RenderData.VertexDataPtr->TexCoord = baseTexCoord[i] * quad.TilingFactor;
			s_RenderData.VertexDataPtr->TexIndex = 0; // Default blank texture
			s_RenderData.VertexDataPtr++;
		}
					
		s_RenderData.IndexCount += 6;
	}

	void Renderer2D::SubmitQuad(const TexturedQuad& quad)
	{
		if (s_RenderData.IndexCount >= BatchSettings::MaxIndices) {
			Flush();
			NewBatch();
		}

		uint32_t texIndex = 0;

		for (uint32_t i = 1; i < s_RenderData.TextureSlotIndex; i++) {
			if (s_RenderData.TextureSlots[i]->GetID() == quad.Texture->GetID()) {
				texIndex = i;
				break;
			}
		}

		if (texIndex == 0) {
			if (s_RenderData.TextureSlotIndex >= BatchSettings::MaxTextureSlots) {
				Flush();
				NewBatch();
			}

			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = quad.Texture;
			texIndex = s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlotIndex++;
		}

		glm::mat4 rotation(1.0f);

		if (quad.Rotation != 0.0f) {
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.Rotation), { 0.0f, 0.0f, 1.0f });
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), { quad.Position.x, quad.Position.y, quad.SortingOrder }) *
			rotation *
			glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });

		for (int i = 0; i < 4; i++) {
			s_RenderData.VertexDataPtr->Position = transform * baseQuadPosition[i];
			s_RenderData.VertexDataPtr->Color = quad.Tint;
			s_RenderData.VertexDataPtr->TexCoord = baseTexCoord[i] * quad.TilingFactor;
			s_RenderData.VertexDataPtr->TexIndex = texIndex;
			s_RenderData.VertexDataPtr++;
		}
		
		s_RenderData.IndexCount += 6;
	}

	void Renderer2D::SubmitQuad(const TexturedQuadn& quad)
	{
		if (s_RenderData.IndexCount >= BatchSettings::MaxIndices) {
			Flush();
			NewBatch();
		}

		uint32_t texIndex = 0;

		for (uint32_t i = 1; i < s_RenderData.TextureSlotIndex; i++) {
			if (s_RenderData.TextureSlots[i]->GetID() == quad.Texture->GetID()) {
				texIndex = i;
				break;
			}
		}

		if (texIndex == 0) {
			if (s_RenderData.TextureSlotIndex >= BatchSettings::MaxTextureSlots) {
				Flush();
				NewBatch();
			}

			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = quad.Texture;
			texIndex = s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlotIndex++;
		}

		for (int i = 0; i < 4; i++) {
			s_RenderData.VertexDataPtr->Position = quad.Transform * baseQuadPosition[i];
			s_RenderData.VertexDataPtr->Color = quad.Tint;
			s_RenderData.VertexDataPtr->TexCoord = baseTexCoord[i] * quad.TilingFactor;
			s_RenderData.VertexDataPtr->TexIndex = texIndex;
			s_RenderData.VertexDataPtr++;
		}

		s_RenderData.IndexCount += 6;
	}

	void Renderer2D::SubmitQuad(const SubTexturedQuad& quad)
	{
		if (s_RenderData.IndexCount >= BatchSettings::MaxIndices) {
			Flush();
			NewBatch();
		}

		uint32_t texIndex = 0;

		for (uint32_t i = 1; i < s_RenderData.TextureSlotIndex; i++) {
			if (s_RenderData.TextureSlots[i]->GetID() == quad.Texture->GetAtlas()->GetID()) {
				texIndex = i;
				break;
			}
		}

		if (texIndex == 0) {
			if (s_RenderData.TextureSlotIndex >= BatchSettings::MaxTextureSlots) {
				Flush();
				NewBatch();
			}

			s_RenderData.TextureSlots[s_RenderData.TextureSlotIndex] = quad.Texture->GetAtlas()->GetTexture();
			texIndex = s_RenderData.TextureSlotIndex;
			s_RenderData.TextureSlotIndex++;
		}

		glm::mat4 rotation(1.0f);

		if (quad.Rotation != 0.0f) {
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.Rotation), { 0.0f, 0.0f, 1.0f });
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), { quad.Position.x, quad.Position.y, quad.SortingOrder }) *
			rotation *
			glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });


		const glm::vec2* texCoords = quad.Texture->GetTexCoords();
		for (int i = 0; i < 4; i++) {
			s_RenderData.VertexDataPtr->Position = transform * baseQuadPosition[i];
			s_RenderData.VertexDataPtr->Color = quad.Tint;
			s_RenderData.VertexDataPtr->TexCoord = texCoords[i] * quad.TilingFactor;
			s_RenderData.VertexDataPtr->TexIndex = texIndex;
			s_RenderData.VertexDataPtr++;
		}
		
		s_RenderData.IndexCount += 6;
	}

	Renderer2D::RenderStats Renderer2D::GetStats()
	{
		return s_RenderData.Stats;
	}

}
