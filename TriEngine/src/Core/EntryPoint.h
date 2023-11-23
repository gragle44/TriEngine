#pragma once

#ifdef TRI_PLATFORM_WINDOWS

extern TriEngine::Application * TriEngine::CreateApplication();

int main(int argc, char** argv)
{
	auto app = TriEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif