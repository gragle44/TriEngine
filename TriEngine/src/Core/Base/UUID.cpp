#include "tripch.h"
#include "UUID.h"

#include "Random.h"

namespace TriEngine {
	UUID::UUID()
	{
		uint32_t part1 = Random::Uint(0, UINT32_MAX);
		uint32_t part2 = Random::Uint(0, UINT32_MAX);

		m_ID = static_cast<uint64_t>(part1) << 32 | part2;
	}
}