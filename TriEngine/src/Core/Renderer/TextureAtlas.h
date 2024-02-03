#pragma once

#include "Texture.h"
#include <glm/vec2.hpp>

namespace TriEngine {
	class TextureAtlas {
	public:
		TextureAtlas();
		TextureAtlas(Reference<Texture2D> atlas, uint32_t subTextSize);
		~TextureAtlas() = default;

		const Reference<Texture2D>& GetAtlas() { return m_Atlas; }

		glm::vec2 GetUVCoords(uint32_t xIndex, uint32_t yIndex) const;

		void SetAtlas(Reference<Texture2D> atlas, uint32_t subTextSize);
		void Bind(uint32_t slot) const { m_Atlas->Bind(slot); }

		uint32_t GetID() const { return m_Atlas->GetID(); }
		uint32_t GetWidth() const { return m_Atlas->GetWidth(); }
		uint32_t GetHeight() const { return m_Atlas->GetHeight(); }

	private:
		Reference<Texture2D> m_Atlas;
		uint32_t m_SubTexSize;
	};
}
