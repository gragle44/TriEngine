#pragma once

#if defined(TRI_PLATFORM_WINDOWS) || defined(TRI_PLATFORM_LINUX)

extern TriEngine::Application* TriEngine::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	TriEngine::Log::Init();
	TRI_CORE_INFO("Initialized logger!");

	auto app = TriEngine::CreateApplication(argc, argv);
	app->Run();
	delete app;
}

#endif