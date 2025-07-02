#include "App.h"

#include "TriEngine.h"

#include <imgui.h>
#include <glm/glm.hpp>

using namespace TriEngine;

App::App()
{
}

void App::OnAttach()
{
	RenderCommand::SetClearColor({ 0.15f, 0.15f, 0.15f, 1.0f });

	TriEngine::ResourceManager::Init();

	m_Renderer = std::make_unique<GameRenderer>();

	m_ActiveScene = std::dynamic_pointer_cast<Scene>(ResourceManager::Get(ProjectManager::GetCurrent()->GetProjectData().StartupSceneID));

	auto& windowSettings = ProjectManager::GetCurrentProjectData().WindowSettings;
	m_Renderer->SetViewportSize(windowSettings.Width, windowSettings.Height);
	m_ActiveScene->OnViewportResized(windowSettings.Width, windowSettings.Height);
	
	m_ActiveScene->Start();

}

void App::OnDetach()
{
	m_ActiveScene->Stop();
}

void App::OnUpdate(float deltaTime)
{	
	m_ActiveScene->OnUpdate(deltaTime);
	m_Renderer->RenderScene(m_ActiveScene.get());
}

void App::OnEvent(Event& e)
{
	TriEngine::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<TriEngine::WindowResizeEvent>(TRI_BIND_EVENT_FN(App::OnWindowResized));
}

bool App::OnWindowResized(WindowResizeEvent& e) {
	m_Renderer->SetViewportSize(e.GetWidth(), e.GetHeight());
	RenderCommand::SetViewPort({0, 0, e.GetWidth(), e.GetHeight()});
	m_ActiveScene->OnViewportResized(e.GetWidth(), e.GetHeight());
	return false;
}
