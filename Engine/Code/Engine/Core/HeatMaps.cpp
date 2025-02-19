#include "Engine/Core/HeatMaps.hpp"
#include "Engine/Math/MathUtils.hpp"

TileHeatMap::TileHeatMap(IntVec2 const& dimensions)
	:m_dimensions(dimensions)
{
	int size = dimensions.x * dimensions.y;
	m_values.resize(size);
}

TileHeatMap::~TileHeatMap()
{
}

void TileHeatMap::AddVertsForDebugDraw(std::vector<Vertex_PCU>& verts, AABB2 bounds, FloatRange valueRange, Rgba8 lowColor, Rgba8 highColor, float specialValue, Rgba8 specialColor)
{
	for (int x = 0; x < m_dimensions.x; x++)
	{
		for (int y = 0; y < m_dimensions.y; y++)
		{
			Vertex_PCU topLeft;
			Vertex_PCU topRight;
			Vertex_PCU bottomLeft;
			Vertex_PCU bottomRight;

			float xMinPos = (bounds.m_maxs.x / m_dimensions.x) * x;
			float yMinPos = (bounds.m_maxs.y / m_dimensions.y) * y;
			float xMaxPos = xMinPos + 1.0f;
			float yMaxPos = yMinPos + 1.0f;

			topLeft.m_position = Vec3(xMinPos, yMaxPos, 0);
			topRight.m_position = Vec3(xMaxPos, yMaxPos, 0);
			bottomLeft.m_position = Vec3(xMinPos, yMinPos, 0);
			bottomRight.m_position = Vec3(xMaxPos, yMinPos, 0);

			int index = x + y * m_dimensions.x;

			if (m_values[index] == specialValue)
			{
				topLeft.m_color = specialColor;
				topRight.m_color = specialColor;
				bottomLeft.m_color = specialColor;
				bottomRight.m_color = specialColor;
			}
			else
			{
				float tileValue = RangeMapClamped(m_values[index], valueRange.m_min, valueRange.m_max, 0.f, 1.f);
				topLeft.m_color = Interpolate(lowColor, highColor, tileValue);
				topRight.m_color = Interpolate(lowColor, highColor, tileValue);
				bottomLeft.m_color = Interpolate(lowColor, highColor, tileValue);
				bottomRight.m_color = Interpolate(lowColor, highColor, tileValue);
			}

			verts.push_back(topLeft);
			verts.push_back(topRight);
			verts.push_back(bottomRight);

			verts.push_back(topLeft);
			verts.push_back(bottomLeft);
			verts.push_back(bottomRight);
		}
	}
}

void TileHeatMap::SetAllValues()
{
	for (int i = 0; i < (int)m_values.size(); i++)
	{
		m_values[i] = 0;
	}
}

float TileHeatMap::GetHeatAt(IntVec2 coord)
{
	if (coord.x < 0 || coord.y < 0 || coord.x >= m_dimensions.x || coord.y >= m_dimensions.y)
	{
		return 9999.f;
	}
	int index = coord.x + coord.y * m_dimensions.x;

	return m_values[index];
}

void TileHeatMap::SetHeaEverywhere(float value)
{
	for (int i = 0; i < (int)m_values.size(); i++)
	{
		m_values[i] = value;
	}
}

void TileHeatMap::SetHeatAt(IntVec2 coord, float value)
{
	int index = coord.x + coord.y * m_dimensions.x;

	m_values[index] = value;
}

void TileHeatMap::AddHeatAt(IntVec2 coord, float valueToAdd)
{
	int index = coord.x + coord.y * m_dimensions.x;

	m_values[index] += valueToAdd;
}

float TileHeatMap::GetHighestHeat()
{
	float highestHeat = m_values[0];
	for (int i = 0; i < (int)m_values.size(); i++)
	{
		if (m_values[i] > highestHeat)
		{
			highestHeat = m_values[i];
		}
	}
	return highestHeat;
}
