#pragma once

#include "Core/Base/Core.h"
#include <string>
#include <glm/vec4.hpp>

namespace TriEngine {
	enum class TextureFilter : uint8_t {
		None = 0,
		Linear, Nearest
	};

	enum class TextureWrapMode : uint8_t {
		None = 0,
		Repeat, MirroredRepeat,
		ClampEdge, ClampBorder
	};

	class Texture {
	public:
		virtual ~Texture() = default;

		virtual void Bind(uint32_t slot) const = 0;
		virtual void UnBind(uint32_t slot) const = 0;

		virtual TextureFilter GetFilterMode() const = 0;
		virtual TextureWrapMode GetWrapMode() const = 0;

		virtual ByteBuffer& GetData() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetID() const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Reference<Texture2D> Create(const std::string& filePath);
		static Reference<Texture2D> Create(const glm::vec4& color, uint32_t size);
	};
}
