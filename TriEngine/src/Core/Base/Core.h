#pragma once

#include <memory>
#include <vector>

#ifdef TRI_DEBUG
	#define TRI_ENABLE_ASSERTS
	#ifdef TRI_PLATFORM_WINDOWS
		#define TRI_DEBUG_GL
	#endif
#endif

#define BIT(x) (1 << x)

#define TRI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace TriEngine {
	using ByteBuffer = std::vector<std::byte>;

	template<typename T>
	using Reference = std::shared_ptr<T>;

	template<typename T>
	using Scoped = std::unique_ptr<T>;
}