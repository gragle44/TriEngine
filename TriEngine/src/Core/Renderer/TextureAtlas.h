#pragma once

#include "Texture.h"
#include <glm/vec2.hpp>

namespace TriEngine {
	class SubTexture2D;

	class TextureAtlas {
	public:
		TextureAtlas() = default;
		TextureAtlas(Reference<Texture2D> atlas, uint32_t subTexSize);
		~TextureAtlas() = default;

		const Reference<Texture2D>& GetAtlas() { return m_Atlas; }

		Reference<SubTexture2D> CreateSubTexture(uint32_t xIndex, uint32_t yIndex, glm::vec2 scale = {1.0f, 1.0f});

		void SetAtlas(Reference<Texture2D> atlas, uint32_t subTextSize);
		void Bind(uint32_t slot) const { m_Atlas->Bind(slot); }

		Reference<Texture2D> GetTexture() const { return m_Atlas; }
		uint32_t GetID() const { return m_Atlas->GetID(); }
		uint32_t GetWidth() const { return m_Atlas->GetWidth(); }
		uint32_t GetHeight() const { return m_Atlas->GetHeight(); }

	private:
		friend class SubTexture2D;

		Reference<Texture2D> m_Atlas;
		uint32_t m_SubTexSize;
	};

	class SubTexture2D {
	public:
		SubTexture2D(TextureAtlas* atlas, float xIndex, float yIndex, glm::vec2 scale = { 1.0f, 1.0f });

		void Update(float xIndex, float yIndex, glm::vec2 scale = { 1.0f, 1.0f });

		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		const TextureAtlas* GetAtlas() const { return m_Atlas; }
		void BindAtlasTexture(uint32_t slot) const { m_Atlas->Bind(slot); }

	private:
		TextureAtlas* m_Atlas;
		glm::vec2 m_TexCoords[4];
	};
}
