#include "Example2D.h"

#include <imgui.h>
#include <glm/glm.hpp>

Example2D::Example2D()
{
}

void Example2D::OnAttach()
{
	m_Scene = new TriEngine::Scene();

	m_Texture = TriEngine::Texture2D::Create("assets/chest.png");
	m_Texture2 = TriEngine::Texture2D::Create("assets/test2.png");
	m_CheckerBoard = TriEngine::Texture2D::Create("assets/test.png");
	m_Texture3 = TriEngine::Texture2D::Create({ 0.1f, 0.25f, 0.65f, 1.0f }, 1);

	TriEngine::TextureSettings settings;
	settings.Filter = TriEngine::TextureFilter::Linear;
	m_GradientTexture = TriEngine::Texture2D::Create({ 0.8f, 0.8f, 0.8f, 0.65f }, {0.15f, 0.15f, 0.15f, 0.65f}, 64, settings);

	m_ChestAtlas = std::make_shared<TriEngine::TextureAtlas>(m_Texture, 16);
	m_Chest1 = m_ChestAtlas->CreateSubTexture(0, 0);

	auto& cam = m_Scene->CreateSceneCamera();
	cam.GetComponent<TriEngine::Camera2DComponent>().Camera.SetSize(10.0f);

	m_Character = m_Scene->CreateGameObject("Character");
	m_Character.AddComponent<TriEngine::TransformComponent>(glm::vec3(0.0f, 0.0f, 0.5f));
	m_Character.AddComponent<TriEngine::Sprite2DComponent>(m_Texture2);

	for (int32_t y = 0; y < 100; y++) {
		for (int32_t x = 0; x < 100; x++) {
			auto quad = m_Scene->CreateGameObject(std::to_string(1 + x + y));
			auto& transform = quad.AddComponent<TriEngine::TransformComponent>(glm::vec3(0.11f * x, 0.11f * y, 0.0f));
			transform.Scale = {0.1f, 0.1f, 0.1};

			auto& sprite = quad.AddComponent<TriEngine::Sprite2DComponent>(m_GradientTexture);
			sprite.Tint = { TriEngine::Random::Float(), TriEngine::Random::Float(), TriEngine::Random::Float(), 1.0f };
		}
	}
	TRI_TRACE("Done loading scene!");

	TriEngine::RenderCommand::SetClearColor({ 0.45f, 0.45f, 0.45f, 1.0f });
}

void Example2D::OnDetach()
{
	delete m_Scene;
}

void Example2D::OnUpdate(float deltaTime)
{
	m_Scene->OnUpdate(deltaTime);
}

void Example2D::OnImGuiRender()
{
}

void Example2D::OnEvent(TriEngine::Event& e)
{
	TriEngine::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<TriEngine::WindowResizeEvent>(TRI_BIND_EVENT_FN(Example2D::OnWindowResized));
}

bool Example2D::OnWindowResized(TriEngine::WindowResizeEvent& e) {
	m_Scene->OnViewportResized(e.GetWidth(), e.GetHeight());

	return false;
}
