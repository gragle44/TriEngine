#include "tripch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	Renderer2D::RenderData* Renderer2D::s_RenderData = new RenderData();

	void Renderer2D::Init()
	{
		s_RenderData->MaxTextureSlots = RenderCommand::GetMaxTextureSlots();
		s_RenderData->TextureSlots.resize(s_RenderData->MaxTextureSlots);

		s_RenderData->VertexArray = VertexArray::Create();

		s_RenderData->VertexData.resize(s_RenderData->MaxVertices);
		s_RenderData->VertexDataPtr = s_RenderData->VertexData.begin();

		s_RenderData->VertexBuffer = VertexBuffer::Create(s_RenderData->MaxVertices * sizeof(QuadVertex));

		{
			TriEngine::BufferLayout layout = {
				{ "a_Position", TriEngine::ShaderDataType::Float3 },
				{ "a_Color", TriEngine::ShaderDataType::Float4 },
				{ "a_TexCoord", TriEngine::ShaderDataType::Float2 },
				{ "a_TexIndex", TriEngine::ShaderDataType::Int}
			};

			s_RenderData->VertexBuffer->SetLayout(layout);
		}	

		uint32_t* quadIndices = new uint32_t[s_RenderData->MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_RenderData->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Reference<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, s_RenderData->MaxIndices);

		s_RenderData->VertexArray->AddVertexAndIndexBuffers(s_RenderData->VertexBuffer, indexBuffer);

		delete[] quadIndices;

		int* samplers = new int[s_RenderData->MaxTextureSlots];

		for (uint32_t i = 0; i < s_RenderData->MaxTextureSlots; i++)
			samplers[i] = i;


		s_RenderData->MainShader = Shader::Create("TextureShader", "src/Shaders/basicvert.glsl", "src/Shaders/basicfrag.glsl");

		s_RenderData->MainShader->Bind();
		s_RenderData->MainShader->SetIntArray("u_Samplers", samplers, s_RenderData->MaxTextureSlots);

		delete[] samplers;

		s_RenderData->DefaultTexture = Texture2D::Create(glm::vec4(1.0f), 1);
		s_RenderData->TextureSlots[0] = s_RenderData->DefaultTexture;
	}

	void Renderer2D::ShutDown()
	{
		delete s_RenderData;
	}

	void Renderer2D::Begin(const OrthographicCamera& camera)
	{

		s_RenderData->Stats.Reset();
		s_RenderData->MainShader->Bind();
		s_RenderData->MainShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		NewBatch();
	}

	void Renderer2D::End()
	{
		uint32_t size = (uint32_t)std::distance(s_RenderData->VertexData.begin(), s_RenderData->VertexDataPtr);

		s_RenderData->VertexBuffer->SetData(s_RenderData->VertexData.data(), size * sizeof(QuadVertex));

		Flush();
	}

	void Renderer2D::Flush()
	{
		for (uint32_t i = 0; i < s_RenderData->TextureSlotIndex; i++) {
			s_RenderData->TextureSlots[i]->Bind(i);
		}

		s_RenderData->VertexArray->Bind();
		RenderCommand::DrawElements(s_RenderData->VertexArray, s_RenderData->IndexCount);
		s_RenderData->Stats.DrawCount++;
	}

	void Renderer2D::NewBatch()
	{
		s_RenderData->Stats.QuadCount += s_RenderData->IndexCount / 6;

		s_RenderData->VertexDataPtr = s_RenderData->VertexData.begin();
		s_RenderData->IndexCount = 0;

		s_RenderData->TextureSlotIndex = 1;
	}

	void Renderer2D::SubmitQuad(const ColoredQuad& quad)
	{
		if (s_RenderData->IndexCount >= s_RenderData->MaxIndices) {
			End();
			NewBatch();
		}

		constexpr glm::vec4 basePos1 = { -0.5f, -0.5f, 0.0f, 1.0f };
		constexpr glm::vec4 basePos2 = {  0.5f, -0.5f, 0.0f, 1.0f };
		constexpr glm::vec4 basePos3 = {  0.5f,  0.5f, 0.0f, 1.0f };
		constexpr glm::vec4 basePos4 = { -0.5f,  0.5f, 0.0f, 1.0f };

		glm::mat4 rotation(1.0f);

		if (((int32_t)quad.Rotation % 360) != 0) {
			rotation = glm::rotate(glm::mat4(1.0f), glm::radians(quad.Rotation), { 0.0f, 0.0f, 1.0f });
		}

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), { quad.Position.x, quad.Position.y, quad.SortingOrder }) *
			rotation *
			glm::scale(glm::mat4(1.0f), { quad.Size.x, quad.Size.y, 1.0f });

		s_RenderData->VertexDataPtr->Position = transform * basePos1;
		s_RenderData->VertexDataPtr->Color = quad.Color;
		s_RenderData->VertexDataPtr->TexCoord = { 0.0f, 0.0f };
		s_RenderData->VertexDataPtr->TexIndex = 0; //Use the default texture
		s_RenderData->VertexDataPtr++;

		s_RenderData->VertexDataPtr->Position = transform * basePos2;
		s_RenderData->VertexDataPtr->Color = quad.Color;
		s_RenderData->VertexDataPtr->TexCoord = { 1.0f, 0.0f };
		s_RenderData->VertexDataPtr->TexIndex = 0; //Use the default texture
		s_RenderData->VertexDataPtr++;

		s_RenderData->VertexDataPtr->Position = transform * basePos3;
		s_RenderData->VertexDataPtr->Color = quad.Color;
		s_RenderData->VertexDataPtr->TexCoord = { 1.0f, 1.0f };
		s_RenderData->VertexDataPtr->TexIndex = 0; //Use the default texture
		s_RenderData->VertexDataPtr++;

		s_RenderData->VertexDataPtr->Position = transform * basePos4;
		s_RenderData->VertexDataPtr->Color = quad.Color;
		s_RenderData->VertexDataPtr->TexCoord = { 0.0f, 1.0f };
		s_RenderData->VertexDataPtr->TexIndex = 0; //Use the default texture
		s_RenderData->VertexDataPtr++;

		s_RenderData->IndexCount += 6;
	}

	void Renderer2D::SubmitQuad(const TexturedQuad& quad)
	{
		if (s_RenderData->IndexCount >= s_RenderData->MaxIndices) {
			Flush();
			NewBatch();
		}

		uint32_t texIndex = 0;

		for (uint32_t i = 1; i < s_RenderData->TextureSlotIndex; i++) {
			if (s_RenderData->TextureSlots[i]->GetID() == quad.Texture->GetID()) {
				texIndex = i;
				break;
			}
		}

		if (texIndex == 0) {
			s_RenderData->TextureSlots[s_RenderData->TextureSlotIndex] = quad.Texture;
			texIndex = s_RenderData->TextureSlotIndex;
			s_RenderData->TextureSlotIndex++;
		}

		s_RenderData->VertexDataPtr->Position = { quad.Position.x, quad.Position.y, quad.SortingOrder };
		s_RenderData->VertexDataPtr->Color = quad.Tint;
		s_RenderData->VertexDataPtr->TexCoord = { 0.0f, 0.0f };
		s_RenderData->VertexDataPtr->TexIndex = texIndex;
		s_RenderData->VertexDataPtr++;

		s_RenderData->VertexDataPtr->Position = { quad.Position.x + quad.Size.x, quad.Position.y, quad.SortingOrder };
		s_RenderData->VertexDataPtr->Color = quad.Tint;
		s_RenderData->VertexDataPtr->TexCoord = { 1.0f, 0.0f };
		s_RenderData->VertexDataPtr->TexIndex = texIndex;
		s_RenderData->VertexDataPtr++;

		s_RenderData->VertexDataPtr->Position = { quad.Position.x + quad.Size.x, quad.Position.y + quad.Size.y, quad.SortingOrder };
		s_RenderData->VertexDataPtr->Color = quad.Tint;
		s_RenderData->VertexDataPtr->TexCoord = { 1.0f, 1.0f };
		s_RenderData->VertexDataPtr->TexIndex = texIndex;
		s_RenderData->VertexDataPtr++;

		s_RenderData->VertexDataPtr->Position = { quad.Position.x, quad.Position.y + quad.Size.y, quad.SortingOrder };
		s_RenderData->VertexDataPtr->Color = quad.Tint;
		s_RenderData->VertexDataPtr->TexCoord = { 0.0f, 1.0f };
		s_RenderData->VertexDataPtr->TexIndex = texIndex;
		s_RenderData->VertexDataPtr++;

		s_RenderData->IndexCount += 6;
	}

	Renderer2D::RenderStats Renderer2D::GetStats()
	{
		return s_RenderData->Stats;
	}

}
