#pragma once
#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/DoubleAABB3.hpp"
#include "Engine/Math/DoubleLineSegment3.hpp"
#include "Engine/Math/ConvexShape.hpp"

struct Capsule3;
struct DoubleOBB3;
struct DoubleAABB3;

struct DoubleCapsule3 
{
public:
	DoubleVec3 m_start;
	DoubleVec3 m_end;
	double m_radius;

public:

	~DoubleCapsule3() = default;
	DoubleCapsule3() = default;
	DoubleCapsule3(Capsule3 const& capsule);
	DoubleCapsule3(DoubleCapsule3 const& copyfrom);
	explicit DoubleCapsule3(DoubleVec3 start, DoubleVec3 end, double radius);

	DoubleVec3 GetAxis() const;
	DoubleVec3 GetAxisNormal() const;
	double GetAxisLength() const;
	double GetAxisHalfLength() const;
	DoubleVec3 GetNearestPoint(DoubleVec3 const point) const;
	bool IsPointInside(DoubleVec3 const point) const;
	DoubleAABB3 GetBound() const;
	double GetBoneLength() const;
	void FixLength();
	DoubleLineSegment3 GetBone() const;
	DoubleVec3 GetCenter() const;

	DoubleOBB3 GetOrientedBox() const;
	DoubleAABB3 GetBoundingBox() const;

	//DoubleVec3 support(const DoubleVec3& direction) const override;

private:
	double m_length;
};