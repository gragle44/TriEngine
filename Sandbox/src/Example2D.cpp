#include "Example2D.h"

#include "imgui.h"
#include "glm/glm.hpp"

Example2D::Example2D()
	:m_CameraController(TriEngine::OrthographicCamera(1280, 720))
{
}

void Example2D::OnAttach()
{
	m_Texture = TriEngine::Texture2D::Create("assets/test2.png");
}

void Example2D::OnDetach()
{
}

void Example2D::OnUpdate(float deltaTime)
{
	TriEngine::Quad quad = TriEngine::Quad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, m_Texture);
	TriEngine::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });

	TriEngine::Renderer2D::Begin(m_CameraController.GetCamera());
	TriEngine::Renderer2D::DrawQuad(quad);
	TriEngine::Renderer2D::End(); 
}

void Example2D::OnImGuiRender()
{
}

void Example2D::OnEvent(TriEngine::Event& e)
{
}
