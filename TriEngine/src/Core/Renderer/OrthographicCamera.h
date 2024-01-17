#pragma once

#include "Camera.h"

namespace TriEngine {
	class OrthographicCamera : public Camera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		OrthographicCamera(uint32_t width, uint32_t height);

		virtual ~OrthographicCamera() = default;

		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewProjectionMatrix(); }
		void MovePosition(const glm::vec3& delta) { m_Position += delta; RecalculateViewProjectionMatrix(); }
		glm::vec3 GetPosition() { return m_Position; }

		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewProjectionMatrix(); }
		float GetRotation() const { return m_Rotation; }

		void SetProjection(float left, float right, float bottom, float top);

		void SetNearClip(float nearClip) { m_NearClip = nearClip; RecalculateViewProjectionMatrix(); }
		float GetNearClip() const { return m_NearClip; }
		void SetFarClip(float farClip) { m_FarClip = farClip; RecalculateViewProjectionMatrix(); }
		float GetFarClip() const { return m_FarClip; }

	private:
		void RecalculateViewProjectionMatrix();

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;


		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		float m_Zoom = 1.0f;

		float m_NearClip = -1.0f;
		float m_FarClip = 1.0f;
	};
}
