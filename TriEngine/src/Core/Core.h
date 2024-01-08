#pragma once

#ifdef TRI_PLATFORM_WINDOWS
#if TRI_DYNAMIC_LINK
	#ifdef TRI_BUILD_DLL
		#define TRI_API __declspec(dllexport)
	#else
		#define TRI_API __declspec(dllimport)
		#define IMGUI_API __declspec(dllimport)
	#endif
#else
	#define TRI_API
#endif
#endif

#ifdef TRI_DEBUG
	#define TRI_ENABLE_ASSERTS
#endif


#ifdef TRI_ENABLE_ASSERTS
	#define TRI_ASSERT(x, ...) { if(!(x)) { TRI_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TRI_CORE_ASSERT(x, ...) { if(!(x)) { TRI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define TRI_ASSERT(x, ...)
	#define TRI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define TRI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)