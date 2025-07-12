#pragma once

#include "Core/Renderer/Camera.h"
#include "Core/Events/Event.h"
#include "Core/Events/MouseEvent.h"

#include <glm/gtc/matrix_transform.hpp>

namespace TriEngine {
	class EditorCamera : public Camera {
	public:
		EditorCamera();
		virtual ~EditorCamera() override = default;

		void OnUpdate(float deltaTime);
		void OnEvent(Event& e);

		bool OnMouseScrolled(MouseScrolledEvent& e);

		glm::vec2 MoveSpeed();

		void SetSize(float zoom, float nearClip = -1.0f, float farClip = 1.0f);
		void SetViewportSize(uint32_t width, uint32_t height, uint32_t windowHeight = 0);

		glm::mat4 GetTransform() const {
			return glm::translate(glm::mat4(1.0f), m_Position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), { 0, 0, 1 })
				* glm::scale(glm::mat4(1.0f), { m_Scale.x, m_Scale.y, 1.0f });
		}

	private:
		void RecalculateProjection();

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
		glm::vec2 m_Scale = { 1.0f, 1.0f };

		float m_ViewportWidth;
		float m_ViewportHeight;

		float m_Zoom = 10.0f;
		float m_AspectRatio = 0.0f;
		float m_YScale = 0.0f;
		float m_NearClip = -1.0f, m_FarClip = 1.0f;

		bool m_Panning = false;
		glm::vec2 m_LastMousePos = { 0, 0 };

		friend class SceneModule;
		friend class SceneSerializer;
	};
}