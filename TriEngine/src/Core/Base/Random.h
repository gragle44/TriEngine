#pragma once

#include <random>
#include "tripch.h"

namespace TriEngine {
	class Random {
	public:
		static float Float(float min = 0.0f, float max = 1.0f);

		static int32_t Int(int32_t min, int32_t max);
		static uint32_t Uint(uint32_t min, uint32_t max);

		static bool Bool();
	private:
		template<class T>
		static T::result_type Generate(T distribution);

		thread_local static std::random_device s_Device;
		thread_local static std::mt19937 s_Engine;
	};
}