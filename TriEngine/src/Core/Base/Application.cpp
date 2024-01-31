#include "tripch.h"
#include "Application.h"
#include "Core.h"
#include "Assert.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

#include "Input.h"

namespace TriEngine {
	Application* Application::s_Instance = nullptr;

	Application::Application()
		:m_Running(true), m_Minimized(false)
	{
		TRI_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(TRI_BIND_EVENT_FN(Application::OnEvent));

		InitRenderer();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		
	}

	void Application::Run()
	{
		while (m_Running)
		{
			if (!m_Minimized) {
				m_DeltaTime.Update();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(m_DeltaTime);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
		TRI_CORE_INFO("Shutting Down!");
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(TRI_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(TRI_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		RenderCommand::SetViewPort({ 0, 0, e.GetWidth(), e.GetHeight() });
		m_Minimized = false;
		return false;

	}

	void Application::InitRenderer()
	{
		RenderCommand::Init();
		Renderer::Init();
		Renderer2D::Init();
	}
}