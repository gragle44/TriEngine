#include "tripch.h"
#include "Renderer2D.h"

#include "RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	Renderer2D::RenderData* Renderer2D::s_RenderData = new RenderData();

	void Renderer2D::Init()
	{
		RenderCommand::Init();
		s_RenderData->QuadVertexArray = VertexArray::Create();

		float vertices[] = {
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Reference<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));

		{
			TriEngine::BufferLayout layout = {
				{ "a_Position", TriEngine::ShaderDataType::Float3 },
				{ "a_TexCoord", TriEngine::ShaderDataType::Float2 }
			};

			vertexBuffer->SetLayout(layout);
		}

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Reference<IndexBuffer> indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		s_RenderData->QuadVertexArray->AddVertexAndIndexBuffers(vertexBuffer, indexBuffer);

		s_RenderData->MainShader = Shader::Create("TextureShader", "src/Shaders/basicvert.glsl", "src/Shaders/basicfrag.glsl");

		s_RenderData->MainShader->Bind();
		s_RenderData->MainShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Begin(const OrthographicCamera& camera)
	{
		s_RenderData->MainShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::End()
	{
	}

	void Renderer2D::DrawQuad(const Quad& quad)
	{
		quad.Texture->Bind(0);

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), { quad.Position.x, quad.Position.y, 0.0f }) * 
			glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });

		s_RenderData->MainShader->SetMat4("u_Model", transform);

		s_RenderData->QuadVertexArray->Bind();
		RenderCommand::DrawElements(s_RenderData->QuadVertexArray);
	}

}
