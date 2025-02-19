#pragma once
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include <vector>

class TileHeatMap
{
public:
	TileHeatMap(IntVec2 const& dimensions);
	~TileHeatMap();

	IntVec2 m_dimensions;
	std::vector<float> m_values;


	void AddVertsForDebugDraw(std::vector<Vertex_PCU>& verts, AABB2 bounds, FloatRange valueRange = FloatRange(0.f, 1.f), Rgba8 lowColor = Rgba8(0, 0, 0, 100), Rgba8 highColor = Rgba8(255, 255, 255, 100), float specialValue = 999999.f, Rgba8 specialColor = Rgba8(255, 0, 255));
	void SetAllValues();

	float GetHeatAt(IntVec2 coord);
	void SetHeaEverywhere(float value);
	void SetHeatAt(IntVec2 coord, float value);
	void AddHeatAt(IntVec2 coord, float valueToAdd);

	float GetHighestHeat();

private:

};

