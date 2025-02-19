#define STB_IMAGE_IMPLEMENTATION // Exactly one .CPP (this Image.cpp) should #define this before #including stb_image.h
#include "Engine/Core/Image.hpp"
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/ErrorWarningAssert.hpp"

Image::Image()
{
}

Image::Image(char const* imageFilePath)
	:m_imageFilePath(imageFilePath)
{
	IntVec2 imageDim;
	int bytesPerTexel = 0;
	int numCompsRequested = 0;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* texel = stbi_load(m_imageFilePath.c_str(), &imageDim.x, &imageDim.y, &bytesPerTexel, numCompsRequested);
	if (!texel)
	{
		ERROR_AND_DIE("IMAGE DOESN'T EXIST IN FOLDER");
	}
	m_dimensions = IntVec2(imageDim.x, imageDim.y);
	int totalTexels = imageDim.x * imageDim.y;

	for (int i = 0; i < totalTexels * bytesPerTexel; i += bytesPerTexel)
	{
		if (bytesPerTexel == 3)
		{
			m_rgba8TexelsData.push_back(Rgba8(texel[i], texel[i + 1], texel[i + 2], 255));
		}
		else if (bytesPerTexel == 4)
		{
			m_rgba8TexelsData.push_back(Rgba8(texel[i], texel[i + 1], texel[i + 2], texel[i + 3]));
		}
	}
}

Image::Image(IntVec2 size, Rgba8 color)
	:m_dimensions(size)
{
	int totalSize = size.x * size.y;
	m_rgba8TexelsData.resize(totalSize);

	for (int i = 0; i < totalSize; i++)
	{
		m_rgba8TexelsData[i] = color;
	}

}

Image::~Image()
{
}

std::string const& Image::GetImageFilePath() const
{
	return m_imageFilePath;
}

IntVec2 Image::GetDimensions() const
{
	return m_dimensions;
}

Rgba8 Image::GetTexelColor(IntVec2 const& texelCoords) const
{
	if (texelCoords.x < 0 || texelCoords.x >= m_dimensions.x || texelCoords.y < 0 || texelCoords.y >= m_dimensions.y) {
		return Rgba8(0, 0, 0, 0);
	}
	int index = texelCoords.x + texelCoords.y * m_dimensions.x;
	return m_rgba8TexelsData[index];
}

void Image::SetTexelColor(IntVec2 const& texelCoords, Rgba8 const& newColor)
{
	int index = texelCoords.x + texelCoords.y * m_dimensions.x;
	if (texelCoords.x < 0 || texelCoords.x >= m_dimensions.x || texelCoords.y < 0 || texelCoords.y >= m_dimensions.y) {
		m_rgba8TexelsData[index] = Rgba8(0, 0, 0, 0);
		return;
	}
	m_rgba8TexelsData[index] = newColor;
}

const void* Image::GetRawData() const
{
	return m_rgba8TexelsData.data();
}
