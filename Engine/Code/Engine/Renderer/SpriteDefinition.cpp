#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Window/Window.hpp"

SpriteDefinition::SpriteDefinition(SpriteSheet const& spriteSheet, int spriteIndex, Vec2 const& uvAtMins, Vec2 const& uvAtMaxs)
	:m_spriteSheet(spriteSheet)
	,m_spriteIndex(spriteIndex)
	,m_uvAtMins(uvAtMins)
	,m_uvAtMaxs(uvAtMaxs)
{

}

void SpriteDefinition::GetUVs(Vec2& out_uvAtMins, Vec2& out_uvAtMaxs) const
{
	out_uvAtMaxs = m_uvAtMaxs;
	out_uvAtMins = m_uvAtMins;
}

AABB2 SpriteDefinition::GetUVs() const
{
	return AABB2(m_uvAtMins, m_uvAtMaxs);
}

SpriteSheet const& SpriteDefinition::GetSpriteSheet() const
{
	// TODO: insert return statement here
	return m_spriteSheet;
}

Texture const& SpriteDefinition::GetTexture() const
{
	// TODO: insert return statement here
	return m_spriteSheet.GetTexture();
}

float SpriteDefinition::GetAspect() const
{
	return Window::GetMainWindowInstance()->GetAspect();
}
