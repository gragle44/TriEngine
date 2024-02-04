#include "tripch.h"
#include "TextureAtlas.h"

#include "Base/Assert.h"

namespace TriEngine {
	TextureAtlas::TextureAtlas(Reference<Texture2D> atlas, uint32_t subTexSize)
		:m_Atlas(atlas), m_SubTexSize(subTexSize) {}

	Reference<SubTexture2D> TextureAtlas::CreateSubTexture(uint32_t xIndex, uint32_t yIndex, glm::vec2 scale)
	{
		return std::make_shared<SubTexture2D>(this, xIndex, yIndex, scale);
	}

	void TextureAtlas::SetAtlas(Reference<Texture2D> atlas, uint32_t subTextSize)
	{
		m_Atlas = atlas;
		m_SubTexSize = subTextSize;
	}

	SubTexture2D::SubTexture2D(TextureAtlas* atlas, float xIndex, float yIndex, glm::vec2 scale)
		:m_Atlas(atlas)
	{
		TRI_CORE_ASSERT(atlas, "TextureAtlas was null!");
		m_TexCoords[0] = {  xIndex * (float)atlas->m_SubTexSize / (float)atlas->GetWidth(), (yIndex * (float)atlas->m_SubTexSize) / (float)atlas->GetHeight() };
		m_TexCoords[1] = { (xIndex + scale.x) * (float)atlas->m_SubTexSize / (float)atlas->GetWidth(), yIndex * (float)atlas->m_SubTexSize / (float)atlas->GetHeight() };
		m_TexCoords[2] = { (xIndex + scale.x) * (float)atlas->m_SubTexSize / (float)atlas->GetWidth(), (yIndex + scale.y) * (float)atlas->m_SubTexSize / (float)atlas->GetHeight() };
		m_TexCoords[3] = {  xIndex * (float)atlas->m_SubTexSize / (float)atlas->GetWidth(), (yIndex + scale.y) * (float)atlas->m_SubTexSize / (float)atlas->GetHeight() };
	}

	void SubTexture2D::Update(float xIndex, float yIndex, glm::vec2 scale)
	{
		m_TexCoords[0] = { xIndex * (float)m_Atlas->m_SubTexSize / (float)m_Atlas->GetWidth(), (yIndex * (float)m_Atlas->m_SubTexSize) / (float)m_Atlas->GetHeight() };
		m_TexCoords[1] = { (xIndex + scale.x) * (float)m_Atlas->m_SubTexSize / (float)m_Atlas->GetWidth(), yIndex * (float)m_Atlas->m_SubTexSize / (float)m_Atlas->GetHeight() };
		m_TexCoords[2] = { (xIndex + scale.x) * (float)m_Atlas->m_SubTexSize / (float)m_Atlas->GetWidth(), (yIndex + scale.y) * (float)m_Atlas->m_SubTexSize / (float)m_Atlas->GetHeight() };
		m_TexCoords[3] = { xIndex * (float)m_Atlas->m_SubTexSize / (float)m_Atlas->GetWidth(), (yIndex + scale.y) * (float)m_Atlas->m_SubTexSize / (float)m_Atlas->GetHeight() };
	}
}