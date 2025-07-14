#include "tripch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace TriEngine {
	struct OpenGLRenderData {
		RendererCapabilities RenderCaps;
	};

	OpenGLRenderData* s_RenderData;

	void OpenGLRendererAPI::Init()
	{
		s_RenderData = new OpenGLRenderData();
		RendererCapabilities& caps = s_RenderData->RenderCaps;
		caps.Vendor = (const char*)glGetString(GL_VENDOR);
		caps.Device = (const char*)glGetString(GL_RENDERER);
		caps.Version = (const char*)glGetString(GL_VERSION);

		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &caps.MaxFramebufferSize);
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &caps.MaxSamplers);
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &caps.MaxColorAttachments);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &caps.MaxComputeGroupSizeX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &caps.MaxComputeGroupSizeY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &caps.MaxComputeGroupSizeZ);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);

		//Can be overriden in client
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void OpenGLRendererAPI::Shutdown()
	{
		delete s_RenderData;
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

	void OpenGLRendererAPI::SetLineWidth(uint32_t width) const 
	{
		glLineWidth(width);
	}

	void OpenGLRendererAPI::DepthTest(bool enable) const
	{
		if (enable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void TriEngine::OpenGLRendererAPI::ClearColor(const glm::vec4& color) const
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::MemoryBarrier() const{
		//TODO: Expand functionality
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void OpenGLRendererAPI::SetViewPort(const glm::ivec4& dimensions) const
	{
		glViewport(dimensions.x, dimensions.y, dimensions.z, dimensions.w);
	}

	void TriEngine::OpenGLRendererAPI::DrawElements(const Reference<VertexArray>& vertexArray, uint32_t elementCount) const
	{
		vertexArray->Bind();
		uint32_t count = elementCount ? elementCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawArrays(const Reference<VertexArray>& vertexArray, uint32_t vertexBufferIndex, uint32_t vertexCount) const
	{
		vertexArray->Bind();
		uint32_t count = vertexCount ? vertexCount : vertexArray->GetVertexBuffers()[vertexBufferIndex]->GetVertexCount();

		uint32_t vbOffset = 0;
		for (uint32_t x = 0; x < vertexBufferIndex; x++) {
			vbOffset += vertexArray->GetVertexBuffers()[x]->GetSize();
		}

		glDrawArrays(GL_TRIANGLES, vbOffset, count);
	}

	void OpenGLRendererAPI::DrawLines(const Reference<VertexArray>& vertexArray, uint32_t vertexBufferIndex, uint32_t vertexCount) const 
	{	
		vertexArray->Bind();;
		uint32_t count = vertexCount ? vertexCount : vertexArray->GetVertexBuffers()[vertexBufferIndex]->GetVertexCount();

		uint32_t vbOffset = 0;
		for (uint32_t x = 0; x < vertexBufferIndex; x++) {
			vbOffset += vertexArray->GetVertexBuffers()[x]->GetSize();
		}

		glDrawArrays(GL_LINES, vbOffset, count);
	}

	void OpenGLRendererAPI::DrawElementsInstanced(const Reference<VertexArray>& vertexArray, uint32_t elementCount, uint32_t instanceCount) const
	{
		vertexArray->Bind();
		uint32_t count = elementCount ? elementCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);
	}

	uint32_t OpenGLRendererAPI::GetMaxTextureSlots() const
	{
		int32_t maxTextureUnits;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
		return maxTextureUnits;
	}

	RendererCapabilities& OpenGLRendererAPI::GetRendererCaps()
	{
		return s_RenderData->RenderCaps;
	}

}