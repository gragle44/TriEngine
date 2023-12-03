#include "Application.h"

#include "Log.h"
#include "Events/ApplicationEvent.h"

namespace TriEngine {
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent e(1920, 1080);
		TRI_TRACE(e);

		while (true);
	}
}