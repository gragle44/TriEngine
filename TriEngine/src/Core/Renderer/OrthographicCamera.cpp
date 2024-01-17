#include "tripch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	//Manually construct projection
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		RecalculateViewProjectionMatrix();
	}

	//Construct projection with viewport
	OrthographicCamera::OrthographicCamera(uint32_t width, uint32_t height)
	{
		m_ProjectionMatrix = glm::ortho(-(float)width / 560, (float)width / 560, -(float)height / 560, (float)height / 560, -1.0f, 1.0f);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		RecalculateViewProjectionMatrix();
	}

	void OrthographicCamera::RecalculateViewProjectionMatrix()
	{
		m_Position *= m_Zoom;
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}