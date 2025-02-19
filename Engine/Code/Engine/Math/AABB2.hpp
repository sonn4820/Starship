#pragma once
#include "Engine/Math/Vec2.hpp"
#include <vector>

struct AABB2
{

public:
	Vec2 m_mins;
	Vec2 m_maxs;

public:

	~AABB2() = default;
	AABB2() = default;
	AABB2(AABB2 const& copyfrom);
	explicit AABB2(float minX, float minY, float maxX, float maxY);
	explicit AABB2(Vec2 const& mins, Vec2 const& maxs);
	explicit AABB2(Vec2 const& center, float height, float width);

	bool IsPointInside(Vec2 const& point) const;
	Vec2 const GetCenter() const;
	Vec2 const GetDimensions() const;
	Vec2 const GetHalfDimensions() const;
	Vec2 const GetNearestPoint(Vec2 const& referencePosition) const;
	Vec2 const GetPointAtUV(Vec2 const& uv) const; //(0,0) min, (1,1) max 
	Vec2 const GetUVForPoint(Vec2 const& point) const; // (.5,.5) at center, u or v outside [0,1] extrapolated
	AABB2 const GetAABBAtUVs(Vec2 const& uvMin, Vec2 const& uvMax);

	std::vector<Vec2> GetVertices() const;

	void Translate(Vec2 const& translationToApply);
	void SetCenter(Vec2 const& newCenter);
	void SetDimensions(Vec2 const& newDimensions);
	AABB2 Scale(float zeroToOne);
	AABB2 AddOffSet(float value);
	void StretchToIncludePoint(Vec2 const& point); //does minimal stretching required

	static std::vector<AABB2> CreatePanes(int numPane, AABB2 box, int rows, int columns, float padX = 0.f, float padY = 0.f);

	static AABB2 ZERO_TO_ONE;
};