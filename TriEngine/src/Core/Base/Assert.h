#pragma once

#include "Core.h"
#include "Log.h"


#if defined(TRI_ENABLE_ASSERTS) && defined(TRI_PLATFORM_WINDOWS)
	#define TRI_ASSERT(x, ...) { if(!(x)) { TRI_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TRI_CORE_ASSERT(x, ...) { if(!(x)) { TRI_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#elif defined(TRI_ENABLE_ASSERTS) && defined(TRI_PLATFORM_LINUX)
	#include <signal.h>
	#define TRI_ASSERT(x, ...) { if(!(x)) { TRI_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
	#define TRI_CORE_ASSERT(x, ...) { if(!(x)) { TRI_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); raise(SIGTRAP); } }
#else
	#define TRI_ASSERT(x, ...)
	#define TRI_CORE_ASSERT(x, ...)
#endif