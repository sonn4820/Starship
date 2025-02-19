#pragma once
#include "Engine/Math/Vec2.hpp"
struct OBB2
{

public:
	Vec2 m_center;
	Vec2 m_iBasisNormal;
	Vec2 m_halfDimensions;

public:

	~OBB2() = default;
	OBB2() = default;
	OBB2(OBB2 const& copyfrom);
	explicit OBB2(Vec2 center, Vec2 iBasisNormal, Vec2 halfDimensions);
	explicit OBB2(Vec2 center, float angle, Vec2 halfDimensions);

	bool IsPointInside(Vec2 const& point) const;
	Vec2 const GetNearestPoint(Vec2 const& referencePosition) const;
	void GetCornerPoints(Vec2* out_fourCornerWorldPositions) const;
	Vec2 GetLocalPosForWorldPos(Vec2 worldPos) const;
};