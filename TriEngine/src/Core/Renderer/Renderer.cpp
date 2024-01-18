#include "tripch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace TriEngine {
	glm::mat4 Renderer::s_ViewProjectionMatrix(1.0f);
	
	void Renderer::Begin(const OrthographicCamera& camera)
	{
		s_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::End()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_ViewProjectionMatrix);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("u_ViewProjection", s_ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->SetMat4("u_Model", transform);

		vertexArray->Bind();
		RenderCommand::DrawElements(vertexArray);
	}

}