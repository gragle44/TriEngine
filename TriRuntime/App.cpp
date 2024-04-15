#include "App.h"

#include <imgui.h>
#include <glm/glm.hpp>

App::App()
{
}

void App::OnAttach()
{
}

void App::OnDetach()
{
}

void App::OnUpdate(float deltaTime)
{
}

void App::OnImGuiRender()
{
}

void App::OnEvent(TriEngine::Event& e)
{
	TriEngine::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<TriEngine::WindowResizeEvent>(TRI_BIND_EVENT_FN(App::OnWindowResized));
}

bool App::OnWindowResized(TriEngine::WindowResizeEvent& e) {
	return false;
}
