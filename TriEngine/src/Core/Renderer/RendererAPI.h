#pragma once

#include <glm/glm.hpp>
#include "VertexArray.h"

namespace TriEngine {
	class RendererAPI {
	public:
		enum class API {
			None = 0,
			OpenGL = 1
		};

		virtual void Init() = 0;

		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const = 0;

		virtual void ClearColor(const glm::vec4& color) const = 0;
		virtual void ClearDepth() const = 0;

		virtual void Clear() const = 0;

		virtual void DrawElements(const std::shared_ptr<VertexArray>& vertexArray) const = 0;

		static API GetApi() { return s_API; }
	private:
		static API s_API;
	};
}