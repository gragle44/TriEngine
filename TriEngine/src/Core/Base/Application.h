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

	struct ApplicationConfiguration {
		std::string Name = "App";
		uint32_t WindowWidth = 1280;
		uint32_t WindowHeight = 720;
		bool Resizable = true;
	};

	class Application
	{
	public:
		Application(const ApplicationConfiguration& config);
		virtual ~Application() = default;

		void Close() { m_Running = false; }

		void Pause() { m_Paused = true; }
		void UnPause() { m_Paused = false; }

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		const ApplicationConfiguration& GetConfig() const { return m_Config; }

		Window& GetWindow() { return *m_Window; }

		static Application& Get() { return *s_Instance; }

	private:
		void Shutdown();
		void Run();
		void MainUpdate();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void InitRenderer();

		ApplicationConfiguration m_Config;
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		DeltaTime m_DeltaTime;
		bool m_Paused;
		bool m_Running;
		bool m_Minimized;

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

	};

	//Created in client
	Application* CreateApplication(int argc, char** argv);

}

