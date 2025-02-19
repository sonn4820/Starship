#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/ConvexShape.hpp"

struct DoubleOBB3;
struct OBB3 : public ConvexShape
{

public:
	Vec3 m_center;
	Vec3 m_iBasisNormal;
	Vec3 m_jBasisNormal;
	Vec3 m_halfDimensions;

public:

	~OBB3() = default;
	OBB3() = default;
	OBB3(OBB3 const& copyfrom);
	OBB3(DoubleOBB3 const& doubleObb);
	explicit OBB3(Vec3 center, Vec3 iBasisNormal, Vec3 jBasisNormal, Vec3 halfDimensions);

	bool IsPointInside(Vec3 const& point) const;
	Vec3 const GetNearestPoint(Vec3 const& referencePosition) const;
	void GetCornerPoints(Vec3* out_fourCornerWorldPositions) const;

	Vec3 support(const Vec3& direction) const override;
};