#pragma once

#ifdef TRI_PLATFORM_WINDOWS

extern TriEngine::Application* TriEngine::CreateApplication();

int main(int argc, char** argv)
{
	TriEngine::Log::Init();
	TRI_CORE_INFO("Initialized logger!");

	auto app = TriEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif