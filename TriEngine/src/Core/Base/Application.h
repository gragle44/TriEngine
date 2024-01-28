#pragma once

#include "Core.h"

#include "Core/Events/Event.h"
#include "Core/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Window.h"

#include "Deltatime.h"

#include "Core/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace TriEngine {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }

	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void InitRenderer();

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		DeltaTime m_DeltaTime;
		bool m_Running;
		bool m_Minimized;

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

	};

	//Created in client
	Application* CreateApplication();

}

