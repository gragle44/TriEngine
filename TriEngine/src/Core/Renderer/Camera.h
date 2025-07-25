#pragma once

#include "glm/glm.hpp"

namespace TriEngine {
	class Camera {
	public:
		Camera() = default;
		Camera(const Camera& other) = default;
		virtual ~Camera() = default;

		const glm::mat4& GetProjection() const { return m_Projection; }
	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}
