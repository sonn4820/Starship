#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include <vector>

class SpriteDefinition;

class SpriteSheet
{
public:
	explicit SpriteSheet(Texture& texture, IntVec2 const& simpleGridLayout);
	Texture const&						GetTexture() const;
	int									GetNumSprites() const;
	SpriteDefinition const&				GetSpriteDef(int spriteIndex) const;
	void								GetSpriteUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs, int spriteIndex) const;
	AABB2								GetSpriteUVs(int spriteIndex) const;
	int									GetSpriteIndex(IntVec2 coords) const;

protected:

	Texture const& m_texture;
	std::vector<SpriteDefinition>		m_spriteDefs;
	IntVec2 m_layout;
};