#include "tripch.h"
#include "Renderer/Renderer.h"

namespace TriEngine {

	void Renderer::Begin()
	{
	}

	void Renderer::End()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawElements(vertexArray);
	}

}