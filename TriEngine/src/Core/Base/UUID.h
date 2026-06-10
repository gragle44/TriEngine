#pragma once

#include <functional>

#include <spdlog/fmt/bundled/format.h>

namespace TriEngine {
	class UUID {
	public:
		UUID();
		UUID(UUID const&) = default;
		UUID(uint64_t id) noexcept
			:m_ID(id) {}

		operator uint64_t() const noexcept { return m_ID; }

		// auto operator<=>(const UUID&) const = default;

		friend bool operator==(UUID const& lhs, UUID const& rhs) noexcept {
			return lhs.m_ID == rhs.m_ID;
		}
		friend bool operator!=(UUID const& lhs, UUID const& rhs) noexcept {
			return !(lhs == rhs);
		}

		friend bool operator<(UUID const& lhs, UUID const& rhs) noexcept {
        	return lhs.m_ID < rhs.m_ID;
		}
    
	private:
		uint64_t m_ID = 0;
	};
}

template<>
struct std::hash<TriEngine::UUID> {
	size_t operator()(TriEngine::UUID const& uuid) const noexcept {
		return std::hash<uint64_t>()((uint64_t)(uuid));
	}
};

template <>
struct fmt::formatter<TriEngine::UUID> {
	constexpr auto parse(format_parse_context& ctx) {
		return ctx.begin();
	}

	auto format(const TriEngine::UUID& uuid, fmt::format_context& ctx) const {
		return fmt::format_to(ctx.out(), "{}", static_cast<uint64_t>(uuid));
	}
};
