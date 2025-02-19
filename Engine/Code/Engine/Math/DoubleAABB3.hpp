#pragma once
#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/DoubleVec2.hpp"
#include "Engine/Math/ConvexShape.hpp"

struct AABB3;
struct DoubleAABB3
{

public:
	DoubleVec3 m_mins;
	DoubleVec3 m_maxs;

public:

	~DoubleAABB3() = default;
	DoubleAABB3() = default;
	DoubleAABB3(AABB3 const& aabb);
	DoubleAABB3(DoubleAABB3 const& copyfrom);
	DoubleAABB3(DoubleVec3 center, double height, double widthX, double widthY);
	explicit DoubleAABB3(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);
	explicit DoubleAABB3(DoubleVec3 const& mins, DoubleVec3 const& maxs);
	void StretchToIncludePoint(DoubleVec3 const& point);
	void GetCornerPoints(DoubleVec3* out_fourCornerWorldPositions) const;
	DoubleVec3 GetCenter() const;
	DoubleVec2 GetCenterXY() const;
	DoubleVec3 GetNearestEdgePosition(DoubleVec3 const& point) const;
	DoubleVec3 GetDimension() const;
	DoubleVec3 GetHalfDimension() const;


 	bool IsPointInside(DoubleVec3 const& point) const;
	DoubleVec3 const GetNearestPoint(DoubleVec3 const& referencePosition) const;
;

// 	void Translate(DoubleVec3 const& translationToApply);
// 	void SetCenter(DoubleVec3 const& newCenter);
// 	void SetDimensions(DoubleVec3 const& newDimensions);
// 	void StretchToIncludePoint(DoubleVec3 const& point); //does minimal stretching required

	static DoubleAABB3 ZERO_TO_ONE;
};