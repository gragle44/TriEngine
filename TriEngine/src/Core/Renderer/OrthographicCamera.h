#pragma once

#include "Camera.h"
#include <glm/glm.hpp>

namespace TriEngine {
	class OrthographicCamera : Camera {
	public:
		OrthographicCamera();
		virtual ~OrthographicCamera() override = default;

		void SetSize(float size, float nearClip = -1.0f, float farClip = 1.0f);
		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		void RecalculateProjection();

		float m_Size = 1.0f;
		float m_AspectRaio = 0.0f;
		float m_NearClip = -1.0f, m_FarClip = 1.0f;
	};

	class OrthographicCameraOld{
	public:
		OrthographicCameraOld(float left, float right, float bottom, float top, float zoom = 1.0f);
		OrthographicCameraOld(uint32_t width, uint32_t height, float zoom = 1.0f);

		~OrthographicCameraOld() = default;

		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewProjectionMatrix(); }
		void MovePosition(const glm::vec3& delta) { m_Position = m_Position + delta; RecalculateViewProjectionMatrix(); }
		glm::vec3 GetPosition() const { return m_Position; }

		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewProjectionMatrix(); }
		float GetRotation() const { return m_Rotation; }

		void SetZoom(float zoom);
		float GetZoom() const { return m_Zoom; }

		void SetProjection(float left, float right, float bottom, float top);
		void SetProjection(uint32_t width, uint32_t height);

	private:
		void RecalculateViewProjectionMatrix();

		float m_AspectRatio;
		float m_Rotation = 0.0f;
		float m_Zoom = 1.0f;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
	};
}
