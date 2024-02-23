#pragma once

#include "Core.h"

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

namespace TriEngine {
	class Log
	{
	public:
		static void Init();

		static Reference<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		static Reference<spdlog::logger> GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define TRI_CORE_CRITICAL(...) ::TriEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define TRI_CORE_ERROR(...) ::TriEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TRI_CORE_WARN(...)  ::TriEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TRI_CORE_INFO(...)  ::TriEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TRI_CORE_TRACE(...) ::TriEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)

#define TRI_CRITICAL(...)   ::TriEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define TRI_ERROR(...) ::TriEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define TRI_WARN(...)  ::TriEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TRI_INFO(...)  ::TriEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define TRI_TRACE(...) ::TriEngine::Log::GetClientLogger()->trace(__VA_ARGS__)