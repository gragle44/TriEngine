#pragma once

#include "Camera.h"
#include <glm/glm.hpp>

namespace TriEngine {
	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera();
		OrthographicCamera(const OrthographicCamera& other) = default;
		virtual ~OrthographicCamera() override = default;

		void SetSize(float zoom, float nearClip = -1.0f, float farClip = 1.0f);
		void SetViewportSize(uint32_t width, uint32_t height, uint32_t windowHeight = 0);

	private:
		void RecalculateProjection();

		float m_Zoom = 1.0f;
		float m_AspectRatio = 0.0f;
		float m_YScale = 0.0f;
		float m_NearClip = -1.0f, m_FarClip = 1.0f;

		friend class SceneModule;
		friend class SceneSerializer;
		friend class EntitySerializer;
	};
}
