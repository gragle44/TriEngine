#pragma once

#include <Random>

namespace TriEngine {
	class Random {
	public:
		static void Init();

		static float Float();
	private:
		static std::mt19937 s_Engine;
		static std::uniform_int_distribution<uint32_t> s_UintDist;
	};
}