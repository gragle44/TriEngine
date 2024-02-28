#pragma once

#include "Core/Base/Core.h"
#include <string>
#include <glm/glm.hpp>

namespace TriEngine {
	enum class TextureUsage : uint8_t {
		Image,
		TextureArray,
		Depth
	};

	enum class TextureFilter : uint8_t {
		None = 0,
		Linear, Nearest
	};

	enum class TextureWrap : uint8_t {
		None = 0,
		Repeat, MirroredRepeat,
		ClampEdge, ClampBorder
	};

	struct TextureSettings {
		TextureUsage Usage = TextureUsage::Image;
		TextureFilter Filter = TextureFilter::Nearest;
		TextureWrap Wrap = TextureWrap::Repeat;
		uint32_t Samples = 1;
	};

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot) const = 0;
		virtual void UnBind(uint32_t slot) const = 0;

		virtual TextureFilter GetFilterMode() const = 0;
		virtual TextureWrap GetWrapMode() const = 0;

		virtual ByteBuffer& GetData() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual RID GetID() const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Reference<Texture2D> Create(const glm::ivec2& size, const TextureSettings& settings = TextureSettings());

		static Reference<Texture2D> Create(const std::string& filePath, const TextureSettings& settings = TextureSettings());

		static Reference<Texture2D> Create(const glm::vec4& color, uint32_t size, const TextureSettings& settings = TextureSettings());

		static Reference<Texture2D> Create(const glm::vec4& color, const glm::vec4& endColor, uint32_t size, const TextureSettings& settings = TextureSettings());
	};
}
