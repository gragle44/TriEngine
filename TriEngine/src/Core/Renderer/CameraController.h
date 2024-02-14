#pragma once

#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/ApplicationEvent.h"

namespace TriEngine {
	struct CameraSettings {
		float Speed = 1.0f;
		float RotationSpeed = 90.0f;
		float ZoomSpeed = 5.0f;
		float DampingX = 0.0f;
		float DampingY = 0.0f;
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(OrthographicCameraOld camera, const CameraSettings& settings = CameraSettings());

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);

		void Resize(uint32_t width, uint32_t height);

		const OrthographicCameraOld& GetCamera() const { return m_Camera; }
		OrthographicCameraOld& GetCamera() { return m_Camera; }

		const CameraSettings& GetSettings() const { return m_Settings; }
		void SetSettings(CameraSettings settings) { m_Settings = settings; };
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);

		OrthographicCameraOld m_Camera;
		CameraSettings m_Settings;
		glm::vec3 m_Position;
	};
}