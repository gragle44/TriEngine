#include "tripch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	// NEW
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
			m_InitialYValue = (float)windowHeight;
			m_YScale = (float)height / (float)windowHeight;
		}
		else {
			m_InitialYValue = 1.0f;
			m_YScale = 1.0f;
		}
		m_AspectRaio = (float)width / (float)height;
		RecalculateProjection();
	}

	void OrthographicCamera::RecalculateProjection()
	{
		m_Projection = glm::ortho(
			-m_Zoom * m_AspectRaio * 0.5f,  // Bottom
			 m_Zoom * m_AspectRaio * 0.5f,  // Top
			-m_Zoom * m_YScale * 0.5f,                 // Left
			 m_Zoom * m_YScale * 0.5f,                 // Right
			m_NearClip, m_FarClip
		);
	}
	//


	//Manually construct projection
	OrthographicCameraOld::OrthographicCameraOld(float left, float right, float bottom, float top, float zoom)
		:m_AspectRatio(left/bottom), m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), 
		m_ViewMatrix(1.0f), m_Zoom(zoom)
	{
		RecalculateViewProjectionMatrix();
	}

	//Construct projection with viewport
	OrthographicCameraOld::OrthographicCameraOld(uint32_t width, uint32_t height, float zoom)
		:m_AspectRatio(float(width) / (float)height), m_ProjectionMatrix(glm::ortho(-m_AspectRatio * zoom, m_AspectRatio* zoom, -zoom, zoom, -1.0f, 1.0f)),
		m_ViewMatrix(1.0f), m_Zoom(zoom)
	{
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCameraOld::SetZoom(float zoom)
	{
		// Prevent camera inverting
		if (zoom <= 0.0f) {
			return;
		}

		m_Zoom = zoom;
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * zoom, m_AspectRatio * zoom, -zoom, zoom, -1.0f, 1.0f);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCameraOld::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCameraOld::SetProjection(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;
		m_ProjectionMatrix = glm::ortho(-m_AspectRatio * m_Zoom, m_AspectRatio * m_Zoom, -m_Zoom, m_Zoom, -1.0f, 1.0f);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCameraOld::RecalculateViewProjectionMatrix()
	{
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}