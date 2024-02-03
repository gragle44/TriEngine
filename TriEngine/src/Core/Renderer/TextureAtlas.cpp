#include "tripch.h"
#include "TextureAtlas.h"

namespace TriEngine {
	TextureAtlas::TextureAtlas() {}

	TextureAtlas::TextureAtlas(Reference<Texture2D> atlas, uint32_t subTextSize)
		:m_Atlas(atlas), m_SubTexSize(subTextSize) {}

	glm::vec2 TextureAtlas::GetUVCoords(uint32_t xIndex, uint32_t yIndex) const
	{
		return { xIndex * m_SubTexSize / GetWidth(), yIndex * m_SubTexSize / GetHeight() };
	}

	void TextureAtlas::SetAtlas(Reference<Texture2D> atlas, uint32_t subTextSize)
	{
		m_Atlas = atlas;
		m_SubTexSize = subTextSize;
	}
}