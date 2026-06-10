#include "tripch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace TriEngine {
	Reference<spdlog::logger> Log::s_CoreLogger;
	Reference<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T:%e] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("TRIENGINE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Log::SetLevel(LogLevel level) {
		s_CoreLogger->set_level(static_cast<spdlog::level::level_enum>(level));
		s_ClientLogger->set_level(static_cast<spdlog::level::level_enum>(level));
	}
}