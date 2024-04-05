#include "tripch.h"
#include "EditorCamera.h"

#include "Core/Base/Input.h"

namespace TriEngine {
	EditorCamera::EditorCamera()
	{
		RecalculateProjection();
	}

	void EditorCamera::OnUpdate(float deltaTime)
	{
		glm::vec2 moveSpeed = MoveSpeed();

		if (Input::IsKeyPressed(TRI_KEY_LEFT)) {
			m_Position.x -= moveSpeed.x;
		}
		else if (Input::IsKeyPressed(TRI_KEY_RIGHT)) {
			m_Position.x += moveSpeed.x;
		}

		if (Input::IsKeyPressed(TRI_KEY_DOWN)) {
			m_Position.y -= moveSpeed.y;
		}
		if (Input::IsKeyPressed(TRI_KEY_UP)) {
			m_Position.y += moveSpeed.y;
		}
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(TRI_BIND_EVENT_FN(EditorCamera::OnMouseScrolled));
	}

	bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& e)
	{
		float change = e.GetYOffset() * 0.15f;
		m_Zoom -= change;

		if (m_Zoom < 0.5f)
			m_Zoom = 0.5f;

		RecalculateProjection();
		return false;
	}

	glm::vec2 EditorCamera::MoveSpeed()
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 1.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 1.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	void EditorCamera::SetSize(float size, float nearClip, float farClip)
	{
		m_Zoom = size;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		RecalculateProjection();
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height, uint32_t windowHeight)
	{
		if (windowHeight != 0) {
			m_YScale = (float)height / (float)windowHeight;
		}
		else {
			m_YScale = 1.0f;
		}
		m_AspectRatio = (float)width / (float)height;
		m_ViewportWidth = (float)width;
		m_ViewportHeight = (float)height;
		RecalculateProjection();
	}

	void EditorCamera::RecalculateProjection()
	{
		m_Projection = glm::ortho(
			-m_Zoom * m_AspectRatio * 0.5f, // Bottom
			m_Zoom * m_AspectRatio * 0.5f,  // Top
			-m_Zoom * m_YScale * 0.5f,      // Left
			m_Zoom * m_YScale * 0.5f,       // Right
			m_NearClip, m_FarClip
		);
	}
	

}
