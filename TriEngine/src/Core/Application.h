#pragma once

#include "Core.h"
#include "Window.h"

namespace TriEngine {

	class TRI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;

	};

	//Create in client
	Application* CreateApplication();

}

