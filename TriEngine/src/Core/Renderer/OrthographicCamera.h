#pragma once

#include "Camera.h"

namespace TriEngine {
	class OrthographicCamera{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zoom = 1.0f);
		OrthographicCamera(uint32_t width, uint32_t height, float zoom = 1.0f);

		~OrthographicCamera() = default;

		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewProjectionMatrix(); }
		void MovePosition(const glm::vec3& delta) { m_Position = m_Position + delta; RecalculateViewProjectionMatrix(); }
		glm::vec3 GetPosition() const { return m_Position; }

		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewProjectionMatrix(); }
		float GetRotation() const { return m_Rotation; }

		void SetZoom(float zoom);
		float GetZoom() const { return m_Zoom; }

		void SetProjection(float left, float right, float bottom, float top);

	private:
		void RecalculateViewProjectionMatrix();

		glm::mat4 m_ScaleMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_Zoom = 1.0f;
	};
}
