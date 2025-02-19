#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Window/Window.hpp"

SpriteSheet::SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout)
	:m_texture(texture), m_layout(simpleGridLayout)
{
	int size = simpleGridLayout.x * simpleGridLayout.y;
	m_spriteDefs.reserve(size);

	for (int i = 0; i < size; i++)
	{
		int x = i % simpleGridLayout.x;
		int y = i / simpleGridLayout.x;

		y = simpleGridLayout.x - 1 - y + (simpleGridLayout.y - simpleGridLayout.x);

		float u = float(1.0f / (float)simpleGridLayout.x);
		float v = float(1.0f / (float)simpleGridLayout.y);

		IntVec2 sheetDim = m_texture.GetDimensions();
		Vec2 uvCorrection = Vec2(1.f / (static_cast<float>(sheetDim.x) * 128.f), 1.f / (static_cast<float>(sheetDim.y) * 128.f));
		AABB2 uvBox = AABB2(x * u + uvCorrection.x, y * v + uvCorrection.y, (x + 1) * u - uvCorrection.x, (y + 1) * v - uvCorrection.y);

		SpriteDefinition spriteDef = SpriteDefinition(*this, i, uvBox.m_mins, uvBox.m_maxs);
		m_spriteDefs.push_back(spriteDef);
	}

}
Texture const& SpriteSheet::GetTexture() const
{
	return m_texture;
}

int SpriteSheet::GetNumSprites() const
{
	return (int)m_spriteDefs.size();
}

SpriteDefinition const& SpriteSheet::GetSpriteDef(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex];
}

void SpriteSheet::GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const
{
	out_uvAtMins = m_spriteDefs[spriteIndex].GetUVs().m_mins;
	out_uvAtMaxs = m_spriteDefs[spriteIndex].GetUVs().m_maxs;
}

AABB2 SpriteSheet::GetSpriteUVs(int spriteIndex) const
{
	return m_spriteDefs[spriteIndex].GetUVs();
}

int SpriteSheet::GetSpriteIndex(IntVec2 coords) const
{
	return  coords.x + coords.y * m_layout.x;
}