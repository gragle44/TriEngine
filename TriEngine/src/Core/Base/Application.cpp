#include "tripch.h"
#include "Application.h"
#include "Core.h"
#include "Assert.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"

#include "Input.h"

namespace TriEngine {
	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationConfiguration& config)
		:m_Config(config), m_Paused(false), m_Running(true), m_Minimized(false)
	{
		TRI_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(m_Config.Name, m_Config.WindowWidth, m_Config.WindowHeight, m_Config.Resizable)));
		m_Window->SetEventCallback(TRI_BIND_EVENT_FN(Application::OnEvent));

		InitRenderer();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::MainUpdate() 
	{
		if (!m_Paused) {
			m_DeltaTime.Update();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(m_DeltaTime);
		}

		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerStack)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Window->OnUpdate();

		TRI_PROFILE_FRAME;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			MainUpdate();
		}
		Shutdown();
	}

	void Application::Shutdown()
	{
		Renderer2D::ShutDown();
		TRI_CORE_INFO("Shutting Down!");
	}

	void Application::OnEvent(Event& e)
	{
		m_ImGuiLayer->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(TRI_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(TRI_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
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
			m_Paused = true;
			return false;
		}

		RenderCommand::SetViewPort({ 0, 0, e.GetWidth(), e.GetHeight() });
		m_Paused = false;
		return false;

	}

	void Application::InitRenderer()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}
}