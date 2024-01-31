#include "Example2D.h"

#include "imgui.h"
#include "glm/glm.hpp"

Example2D::Example2D()
	:m_CameraController(TriEngine::OrthographicCamera(1280, 720))
{
}

void Example2D::OnAttach()
{
	m_Texture = TriEngine::Texture2D::Create("assets/test.jpg");
	m_Texture2 = TriEngine::Texture2D::Create("assets/test2.png");
	m_CheckerBoard = TriEngine::Texture2D::Create("assets/test.png");
	m_Texture3 = TriEngine::Texture2D::Create({ 0.1f, 0.25f, 0.65f, 1.0f }, 1);
	m_GradientTexture = TriEngine::Texture2D::Create({ 0.8f, 0.8f, 0.8f, 0.65f }, {0.15f, 0.15f, 0.15f, 0.65f}, 64);

	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			m_Quads.push_back({ {0.11f * x, 0.11f * y}, {0.1, 0.1}, m_GradientTexture, {TriEngine::Random::Float(), TriEngine::Random::Float(), TriEngine::Random::Float(), 1.0f}, TriEngine::Random::Float(0.0f, 360.0f) });
		}
	}
}

void Example2D::OnDetach()
{
}

void Example2D::OnUpdate(float deltaTime)
{
	m_CameraController.OnUpdate(deltaTime);
	static TriEngine::TexturedQuad checkerQuad = TriEngine::TexturedQuad({ 0.0f, 0.0f }, { 30.0f, 30.0f }, m_CheckerBoard, glm::vec4(1.0f), 0.0f, -0.5f, 20.0f);
	static TriEngine::TexturedQuad quad2 = TriEngine::TexturedQuad({ -1.25f, -1.25f }, { 1.0f, 1.0f }, m_Texture, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	static TriEngine::TexturedQuad quad3 = TriEngine::TexturedQuad({ -2.5f, -2.5f }, { 1.0f, 1.0f }, m_Texture2, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	quad3.Rotation += -100.0f * deltaTime;

	TriEngine::RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });
	TriEngine::RenderCommand::Clear();

	TriEngine::Renderer2D::Begin(m_CameraController.GetCamera());

	TriEngine::Renderer2D::SubmitQuad(checkerQuad);
	TriEngine::Renderer2D::SubmitQuad(quad2);
	TriEngine::Renderer2D::SubmitQuad(quad3);

	for (const auto& quad : m_Quads) {
		TriEngine::Renderer2D::SubmitQuad(quad);
	} 

	TriEngine::Renderer2D::End(); 
}

void Example2D::OnImGuiRender()
{
}

void Example2D::OnEvent(TriEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}
