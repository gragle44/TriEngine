#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace TriEngine {
	class RendererAPI {
	public:
		enum class API : uint8_t {
			None = 0,
			OpenGL = 1
		};

		virtual void Init() = 0;

		virtual void SetViewPort(const glm::ivec4& dimensions) const = 0;

		virtual void EnableWireframes(bool enabled) const = 0;

		virtual void ClearColor(const glm::vec4& color) const = 0;

		virtual void Clear() const = 0;

		virtual void DrawElements(const Reference<VertexArray>& vertexArray, uint32_t count = 0) const = 0;

		virtual void DrawArrays(const Reference<VertexArray>& vertexArray, uint32_t vertexBufferIndex = 0, uint32_t vertexCount = 0) const = 0;

		virtual uint32_t GetMaxTextureSlots() const = 0;

		static API GetApi() { return s_API; }
	private:
		static API s_API;
	};
}