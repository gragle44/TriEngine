#include "tripch.h"
#include "CameraController.h"

#include "Base/Input.h"

namespace TriEngine {
	OrthographicCameraController::OrthographicCameraController(OrthographicCamera camera, CameraSettings settings)
		:m_Camera(camera), m_Settings(settings), m_Position(0.0f, 0.0f, 0.0f)
	{
	}

	void OrthographicCameraController::OnUpdate(float deltaTime)
	{
		glm::vec3 positionChange(0.0f);
		if (Input::IsKeyPressed(TRI_KEY_W))
			positionChange.y += m_Settings.Speed * deltaTime;
		else if (Input::IsKeyPressed(TRI_KEY_S))
			positionChange.y -= m_Settings.Speed * deltaTime;
		if (Input::IsKeyPressed(TRI_KEY_D))
			positionChange.x += m_Settings.Speed * deltaTime;
		else if (Input::IsKeyPressed(TRI_KEY_A))
			positionChange.x -= m_Settings.Speed * deltaTime;

		m_Camera.MovePosition(positionChange);
	
		if (Input::IsKeyPressed(TRI_KEY_Q))
			m_Camera.SetRotation(m_Camera.GetRotation() + m_Settings.RotationSpeed * deltaTime);
		else if (Input::IsKeyPressed(TRI_KEY_E))
			m_Camera.SetRotation(m_Camera.GetRotation() - m_Settings.RotationSpeed * deltaTime);

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(TRI_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(TRI_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Camera.SetZoom(m_Camera.GetZoom() - e.GetYOffset() * 0.25f);
		return true;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_Camera.SetProjection(e.GetWidth(), e.GetHeight());
		return true;
	}
}