#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace TriEngine {

	class  Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }
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