#include "tripch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	//Manually construct projection
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zoom)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, m_NearClip, m_FarClip)), m_ScaleMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, 1.0f))), m_ViewMatrix(1.0f), m_Zoom(zoom)
	{
		RecalculateViewProjectionMatrix();
	}

	//Construct projection with viewport
	OrthographicCamera::OrthographicCamera(uint32_t width, uint32_t height, float zoom)
		:m_ViewMatrix(1.0f), m_ScaleMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, 1.0f))), m_Zoom(zoom)
	{
		m_ProjectionMatrix = glm::ortho(-(float)width / 480, (float)width / 480, -(float)height / 480, (float)height / 480, m_NearClip, m_FarClip);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::SetZoom(float zoom)
	{
		if (zoom > 0.0f) {
			m_Zoom = zoom;
			m_ScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(m_Zoom, m_Zoom, 1.0f));
			RecalculateViewProjectionMatrix();
		}
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::RecalculateViewProjectionMatrix()
	{
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
			m_ScaleMatrix;
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}