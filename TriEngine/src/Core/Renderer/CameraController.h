#pragma once

#include "Core/Renderer/OrthographicCamera.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/ApplicationEvent.h"

namespace TriEngine {
	struct CameraSettings {
		float Speed = 1.0f;
		float RotationSpeed = 90.0f;
		float ZoomSpeed = 5.0f;
	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(OrthographicCamera camera, CameraSettings settings = CameraSettings());

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);

		void Resize(uint32_t width, uint32_t height);

		const OrthographicCamera& GetCamera() const { return m_Camera; }
		OrthographicCamera& GetCamera() { return m_Camera; }

		const CameraSettings& GetSettings() const { return m_Settings; }
		void SetSettings(CameraSettings settings) { m_Settings = settings; };
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

		OrthographicCamera m_Camera;
		CameraSettings m_Settings;
		glm::vec3 m_Position;
	};
}