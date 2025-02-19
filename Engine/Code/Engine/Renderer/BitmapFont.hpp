#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include <vector>

enum class TextBoxMode
{
	SHRINK,
	OVERRUN
};
class BitmapFont {
public:
	friend class Renderer; // Only the Renderer can create new BitmapFont objects!

private:
	BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture);

public:
	const Texture& GetTexture() const;

	void AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins,
		float cellHeight, std::string const& text, Rgba8 const& tint = Rgba8::COLOR_WHITE, float cellAspect = 1.f, float lineOffset = 0.f, float alignX = 0.f);

	void AddVertsForTextInBox2D(std::vector<Vertex_PCU>&vertexArray, AABB2 const& box, float cellHeight,
		std::string const& text, Rgba8 const& tint = Rgba8::COLOR_WHITE, float cellAspect = 1.f,
		Vec2 const& alignment = Vec2(.5f, .5f), TextBoxMode mode = TextBoxMode::SHRINK, int maxGlyphsToDraw = 99999999);

	void AddVertsForText3DAtOriginXForward(std::vector<Vertex_PCU>& verts, 
		float cellHeight, std::string const& text, 
		Rgba8 const& tint = Rgba8::COLOR_WHITE, float cellAspect = 1.f,
		Vec2 const& alignment = Vec2(.5f, .5f), int maxGlyphsToDraw = 99999999);


	float GetTextWidth(float cellHeight, std::string const& text, float cellAspect = 1.f);

protected:
	float GetGlyphAspect(int glyphUnicode) const; // For now this will always return 1.0f!!!

protected:
	std::string	m_fontFilePathNameWithNoExtension;
	SpriteSheet	m_fontGlyphsSpriteSheet;

};