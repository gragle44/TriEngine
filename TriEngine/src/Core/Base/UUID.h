#pragma once

#include <functional> 

namespace TriEngine {
	class UUID {
	public:
		UUID();
		UUID(uint64_t id)
			:m_ID(id) {}

		operator uint64_t() const { return m_ID; }

		friend bool operator==(UUID const& lhs, UUID const& rhs) noexcept {
			return lhs.m_ID == rhs.m_ID;
		}
		friend bool operator!=(UUID const& lhs, UUID const& rhs) noexcept {
			return !(lhs == rhs);
		}
	private:
		uint64_t m_ID = 0;
	};
}

namespace std {
  template<>
  struct hash<TriEngine::UUID> {
    size_t operator()(TriEngine::UUID const& uuid) const noexcept {
      return std::hash<uint64_t>()((uint64_t)(uuid));
    }
  };
}
