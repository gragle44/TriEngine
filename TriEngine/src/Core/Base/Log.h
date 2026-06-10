#pragma once

#include "Core.h"

#ifdef TRI_PLATFORM_WINDOWS
#pragma warning(push, 0)
#endif

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <spdlog/fmt/bundled/std.h>

#ifdef TRI_PLATFORM_WINDOWS
#pragma warning(pop)
#endif

namespace TriEngine {
	class Log
	{
	public:
		enum class LogLevel : int32_t {
			Trace = SPDLOG_LEVEL_TRACE,
			Debug = SPDLOG_LEVEL_DEBUG,
			Info = SPDLOG_LEVEL_INFO,
			Warn = SPDLOG_LEVEL_WARN,
			Err = SPDLOG_LEVEL_ERROR,
			Critical = SPDLOG_LEVEL_CRITICAL,
			Off = SPDLOG_LEVEL_OFF,
		};

		static void Init();

		static void SetLevel(LogLevel level);

		static Reference<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		static Reference<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

		static void AddSink(spdlog::sink_ptr sink) { s_CoreLogger->sinks().push_back(sink); s_ClientLogger->sinks().push_back(sink); }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

#define TRI_CORE_CRITICAL(...) ::TriEngine::Log::GetCoreLogger()->critical(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_CORE_ERROR(...) ::TriEngine::Log::GetCoreLogger()->error(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_CORE_WARN(...)  ::TriEngine::Log::GetCoreLogger()->warn(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_CORE_INFO(...)  ::TriEngine::Log::GetCoreLogger()->info(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_CORE_TRACE(...) ::TriEngine::Log::GetCoreLogger()->trace(__VA_ARGS__, __FILE__, __LINE__)

#define TRI_CRITICAL(...)   ::TriEngine::Log::GetCoreLogger()->critical(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_ERROR(...) ::TriEngine::Log::GetClientLogger()->error(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_WARN(...)  ::TriEngine::Log::GetClientLogger()->warn(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_INFO(...)  ::TriEngine::Log::GetClientLogger()->info(__VA_ARGS__, __FILE__, __LINE__)
#define TRI_TRACE(...) ::TriEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
