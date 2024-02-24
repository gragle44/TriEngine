#pragma once

#include <memory>
#include <vector>

#ifdef _WIN64
	#define TRI_PLATFORM_WINDOWS
#elif _WIN32
	#error "Only 64 bit Windows is supported"

#elif defined(__APPLE__) || defined(__MACH__)
	#error "Apple is not supported"

#elif defined(__linux__)
	#define TRI_PLATFORM_LINUX
	#error "Linux is not currently supported"

#elif defined (__ANDROID__)
	#define TRI_PLATFORM_ANDROID
	#error "Android is not currently supported"

#else
	#error "Could not detect platform"

#endif

#ifdef TRI_DEBUG
	#define TRI_ENABLE_ASSERTS
#endif

#ifndef TRI_DIST
	#define TRI_DEBUG_GL 1
#else
	#define TRI_DEBUG_GL 0 
#endif


#define BIT(x) (1 << x)

#define TRI_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace TriEngine {
	using ByteBuffer = std::vector<uint8_t>;

	template<typename T>
	using Reference = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T>
	using Scoped = std::unique_ptr<T>;

	using RID = uint32_t;
}