#include "tripch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace TriEngine {

	void TriEngine::OpenGLRendererAPI::ClearColor(const glm::vec4& color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::ClearDepth() const
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void TriEngine::OpenGLRendererAPI::DrawElements(const std::shared_ptr<VertexArray>& vertexArray) const
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

}