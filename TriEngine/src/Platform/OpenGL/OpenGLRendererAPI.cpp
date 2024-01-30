#include "tripch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace TriEngine {
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::EnableWireframes(bool enabled) const
	{
		if (enabled) {
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
		}
	}

	void TriEngine::OpenGLRendererAPI::ClearColor(const glm::vec4& color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetViewPort(const glm::ivec4& dimensions) const
	{
		glViewport(dimensions.x, dimensions.y, dimensions.z, dimensions.w);
	}

	void TriEngine::OpenGLRendererAPI::DrawElements(const Reference<VertexArray>& vertexArray, uint32_t elementCount) const
	{
		uint32_t count = elementCount ? elementCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	uint32_t OpenGLRendererAPI::GetMaxTextureSlots() const
	{
		int32_t maxTextureUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		return maxTextureUnits;
	}

}