#pragma once

#include "Core/Base/Core.h"
#include <string>

namespace TriEngine {
	class Texture {
	public:
		virtual ~Texture() {};

		virtual void Bind(uint32_t slot) const = 0;
		virtual void UnBind(uint32_t slot) const = 0;

		virtual ByteBuffer& GetData() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetID() const = 0;
	};

	class Texture2D : public Texture {
	public:
		static Reference<Texture2D> Create(const std::string& filePath);
	};
}
