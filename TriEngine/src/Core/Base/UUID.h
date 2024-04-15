#pragma once

#include <xhash>

namespace TriEngine {
	class UUID {
	public:
		UUID();
		UUID(uint64_t id)
			:m_ID(id) {}

		operator uint64_t() const { return m_ID; }
	private:
		uint64_t m_ID;
	};
}

namespace std {
	template<>
	struct hash<TriEngine::UUID>
	{
		std::size_t operator()(const TriEngine::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}
