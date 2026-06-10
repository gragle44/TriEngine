#pragma once

#if defined(TRI_PLATFORM_WINDOWS) || defined(TRI_PLATFORM_LINUX)

#ifdef TRI_PLATFORM_WINDOWS
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

extern TriEngine::Application* TriEngine::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	TriEngine::Log::Init();
	TRI_CORE_INFO("Initialized logger!");

	auto app = TriEngine::CreateApplication(argc, argv);
	if (!app) {
		TRI_CORE_ERROR("Failed to create application!");
		return 0;
	}
	app->Run();
	delete app;
}

#endif