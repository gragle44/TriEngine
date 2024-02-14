#pragma once

#include "Core.h"
#include "Log.h"


#ifdef TRI_ENABLE_ASSERTS
	#define TRI_ASSERT(x, ...) { if(!(x)) { TRI_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TRI_CORE_ASSERT(x, ...) { if(!(x)) { TRI_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TRI_ASSERT(x, ...)
	#define TRI_CORE_ASSERT(x, ...)
	#endif