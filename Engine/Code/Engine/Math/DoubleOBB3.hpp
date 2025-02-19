#pragma once
#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/ConvexShape.hpp"

struct OBB3;
struct DoubleAABB3;
struct DoubleOBB3
{

public:
	DoubleVec3 m_center;
	DoubleVec3 m_iBasisNormal;
	DoubleVec3 m_jBasisNormal;
	DoubleVec3 m_halfDimensions;

public:

	~DoubleOBB3() = default;
	DoubleOBB3() = default;
	DoubleOBB3(OBB3 const& obb);
	DoubleOBB3(DoubleOBB3 const& copyfrom);
	explicit DoubleOBB3(DoubleVec3 center, DoubleVec3 iBasisNormal, DoubleVec3 jBasisNormal, DoubleVec3 halfDimensions);

	bool IsPointInside(DoubleVec3 const& point) const;
	DoubleVec3 const GetNearestPoint(DoubleVec3 const& referencePosition) const;
	void GetCornerPoints(DoubleVec3* out_fourCornerWorldPositions) const;

	DoubleAABB3 GetBoundingBox() const;
};