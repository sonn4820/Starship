#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Core/EngineCommon.hpp"

BitmapFont::BitmapFont(char const* fontFilePathNameWithNoExtension, Texture& fontTexture)
	:m_fontFilePathNameWithNoExtension(fontFilePathNameWithNoExtension), m_fontGlyphsSpriteSheet(SpriteSheet(fontTexture, IntVec2(16, 16)))
{

}
const Texture& BitmapFont::GetTexture() const
{
	return m_fontGlyphsSpriteSheet.GetTexture();
}

void BitmapFont::AddVertsForText2D(std::vector<Vertex_PCU>& vertexArray, Vec2 const& textMins, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, float lineOffset, float alignX)
{
	float cellWidth = cellHeight * cellAspect;

	Vec2 cellSize(cellWidth, cellHeight);
	Vec2 cellMins(textMins);

	Strings splittedString = SplitStringOnDelimiter(text, '\n');
	float longestWidth = 0.f;

	for (int i = 0; i < (int)splittedString.size(); i++)
	{
		float width = GetTextWidth(cellHeight, splittedString[i], cellAspect);
		if (width > longestWidth)
		{
			longestWidth = width;
		}
	}

	for (int i = 0; i < (int)splittedString.size(); i++)
	{
		float width = GetTextWidth(cellHeight, splittedString[i], cellAspect);
		if (width > longestWidth)
		{
			longestWidth = width;
		}
	}
	for (int lineIndex = 0; lineIndex < (int)splittedString.size(); lineIndex++)
	{
		float width = GetTextWidth(cellHeight, splittedString[lineIndex], cellAspect);
		float offsetLongest = (longestWidth - width) * alignX;

		std::string textInLine = splittedString[lineIndex];
		cellMins.x = textMins.x + offsetLongest;
		cellMins.y = textMins.y - (cellHeight + lineOffset) * lineIndex  ;

		for (int charIndex = 0; charIndex < (int)textInLine.length(); ++charIndex)
		{
			int glyph = static_cast<int>(textInLine[charIndex]);
			SpriteDefinition spriteDef = m_fontGlyphsSpriteSheet.GetSpriteDef(glyph);
			AABB2 uvBox = spriteDef.GetUVs();


			Vec2 cellMaxs = Vec2(cellMins.x + cellSize.x, cellMins.y + cellSize.y);
			AABB2 textBox = AABB2(cellMins, cellMaxs);
			AddVertsForAABB2D(vertexArray, textBox, tint, uvBox.m_mins, uvBox.m_maxs);
			cellMins.x += cellWidth;
		}
	}

}

void BitmapFont::AddVertsForTextInBox2D(std::vector<Vertex_PCU>& vertexArray, AABB2 const& box, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, TextBoxMode mode, int maxGlyphsToDraw)
{
	std::string textToAdd;

	if ((int)text.size() > maxGlyphsToDraw)
	{
		int counter = 0;
		int maxCounter = maxGlyphsToDraw;

		while (counter < maxCounter)
		{
			if (text[counter] == '\n')
			{
				maxCounter++;
			}

			textToAdd += text[counter];
			counter++;
		}
	}
	else
	{
		textToAdd = text;
	}

	Strings splittedString = SplitStringOnDelimiter(textToAdd, '\n');

	int line = (int)splittedString.size();
	float boxWidth = box.m_maxs.x - box.m_mins.x;
	float boxHeight = box.m_maxs.y - box.m_mins.y;
	std::vector<float> widthScaleList;
	float scaledCellHeight = cellHeight;

	float textBoxHeight = line * cellHeight;
	float scaleH = boxHeight / textBoxHeight;

	switch (mode)
	{
	case TextBoxMode::SHRINK:

		for (int lineIndex = 0; lineIndex < line; lineIndex++)
		{
			float textBoxWidth = GetTextWidth(cellHeight, splittedString[lineIndex], cellAspect);
			float scaleW = boxWidth / textBoxWidth;
			if (scaleW < 1.f)
			{
				float scaledWidth = (cellHeight * cellAspect * scaleW) / cellAspect;
				widthScaleList.push_back(scaledWidth);
			}
		}

		if ((int)widthScaleList.size() > 0)
		{
			scaledCellHeight = widthScaleList[0];
			for (int i = 0; i < (int)widthScaleList.size(); i++)
			{
				if (widthScaleList[i] < scaledCellHeight)
				{
					scaledCellHeight = widthScaleList[i];
				}
			}
		}

		if (scaleH < 1.f)
		{
			float scaledHeight = cellHeight * scaleH;
			if (scaledHeight < scaledCellHeight)
			{
				scaledCellHeight = scaledHeight;
			}
		}

		for (int lineIndex = 0; lineIndex < line; lineIndex++)
		{
			float textBoxWidthAfterScaled = GetTextWidth(scaledCellHeight, splittedString[lineIndex], cellAspect);
			float paddingX = boxWidth - textBoxWidthAfterScaled;

			float textBoxHeightAfterScaled = line * scaledCellHeight;
			float paddingY = boxHeight - textBoxHeightAfterScaled;

			Vec2 textMins = Vec2(box.m_mins.x + (alignment.x * paddingX), box.m_mins.y + (alignment.y * paddingY) + (scaledCellHeight * (line - 1 - lineIndex)));

			AddVertsForText2D(vertexArray, textMins, scaledCellHeight, splittedString[lineIndex], tint, cellAspect);
		}
		break;

	case TextBoxMode::OVERRUN:
		for (int lineIndex = 0; lineIndex < line; lineIndex++)
		{
			float textBoxWidth = GetTextWidth(cellHeight, splittedString[lineIndex], cellAspect);
			float paddingX = boxWidth - textBoxWidth;

			textBoxHeight = line * cellHeight;
			float paddingY = boxHeight - textBoxHeight;

			Vec2 textMins = Vec2(box.m_mins.x + (alignment.x * paddingX), box.m_mins.y + (alignment.y * paddingY) + (cellHeight * (line - 1 - lineIndex)));

			AddVertsForText2D(vertexArray, textMins, cellHeight, splittedString[lineIndex], tint, cellAspect);
		}
		break;
	}
}

void BitmapFont::AddVertsForText3DAtOriginXForward(std::vector<Vertex_PCU>& verts, float cellHeight, std::string const& text, Rgba8 const& tint, float cellAspect, Vec2 const& alignment, int maxGlyphsToDraw)
{
	std::string textToAdd;

	if ((int)text.size() > maxGlyphsToDraw)
	{
		int counter = 0;
		int maxCounter = maxGlyphsToDraw;

		while (counter < maxCounter)
		{
			if (text[counter] == '\n')
			{
				maxCounter++;
			}

			textToAdd += text[counter];
			counter++;
		}
	}
	else
	{
		textToAdd = text;
	}

	Strings splittedString = SplitStringOnDelimiter(textToAdd, '\n');

	int line = (int)splittedString.size();

	for (int lineIndex = 0; lineIndex < line; lineIndex++)
	{
		Vec2 textMins = Vec2(0.f, cellHeight * (line - 1 - lineIndex));

		AddVertsForText2D(verts, textMins, cellHeight, splittedString[lineIndex], tint, cellAspect);
	}

	AABB2 vertsBound = GetVertexBounds2D(verts);
	Mat44 transform;
	transform.AppendXRotation(90);
	transform.AppendYRotation(90);
	transform.AppendTranslation3D(
		Vec3(-(vertsBound.m_maxs.x - vertsBound.m_mins.x)  + alignment.x * (vertsBound.m_maxs.x - vertsBound.m_mins.x)
			, -(vertsBound.m_maxs.y - vertsBound.m_mins.y) + alignment.y * (vertsBound.m_maxs.y - vertsBound.m_mins.y)
			, 0.f)
	);
	TransformVertexArray3D(verts, transform);
}

float BitmapFont::GetTextWidth(float cellHeight, std::string const& text, float cellAspect)
{
	Strings splittedString = SplitStringOnDelimiter(text, '\n');
	float longestTextWidth = 0.f;

	for (int lineIndex = 0; lineIndex < (int)splittedString.size(); lineIndex++)
	{
		float cellWidth = cellHeight * cellAspect;
		float textWidth = 0.f;
		for (int i = 0; i < (int)splittedString[lineIndex].size(); i++)
		{
			textWidth += GetGlyphAspect((int)splittedString[lineIndex][i]) * cellWidth;
		}

		if (textWidth > longestTextWidth)
		{
			longestTextWidth = textWidth;
		}
	}

	return longestTextWidth;
}

float BitmapFont::GetGlyphAspect(int glyphUnicode) const
{
	UNUSED(glyphUnicode);
	return 1.0f;
}
