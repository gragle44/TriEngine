#include "tripch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	OrthographicCamera::OrthographicCamera()
	{
		RecalculateProjection();
	}

	void OrthographicCamera::SetSize(float size, float nearClip, float farClip)
	{
		m_Zoom = size;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		RecalculateProjection();
	}

	void OrthographicCamera::SetViewportSize(uint32_t width, uint32_t height, uint32_t windowHeight)
	{
		if (windowHeight != 0) {
			// If there are ever problems with camera stuff this might be it
			m_YScale = (float)height / (float)windowHeight;
		}
		else {
			m_YScale = 1.0f;
		}
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}

	void OrthographicCamera::RecalculateProjection()
	{
		m_Projection = glm::ortho(
			-m_Zoom * m_AspectRatio * 0.5f,  // Bottom
			m_Zoom * m_AspectRatio * 0.5f,  // Top
			-m_Zoom * m_YScale * 0.5f,       // Left
			m_Zoom * m_YScale * 0.5f,       // Right
			m_NearClip, m_FarClip
		);
	}
}
