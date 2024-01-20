#pragma once

#include "Core/Core.h"
#include <string>

namespace TriEngine {
	class Texture {
	public:
		virtual ~Texture() {};

		virtual void Bind() const = 0;
		virtual void UmBind() const = 0;

		virtual std::vector<std::byte>& GetData() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetID() const = 0;
	};

	class Texture2D {
	public:
		static Reference<Texture2D> Create(const std::string& filePath);
	};
}
