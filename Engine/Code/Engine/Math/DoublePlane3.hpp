#pragma once
#include "Engine/Math/DoubleVec3.hpp"
struct Plane3;
struct DoublePlane3
{

public:
	DoubleVec3 m_normal = DoubleVec3(0, 0, 1);
	double m_distanceFromOrigin = 0;

public:
	DoublePlane3() = default;
	DoublePlane3(const Plane3& plane);
	DoublePlane3(const DoublePlane3& copyfrom);
	explicit DoublePlane3(DoubleVec3 normal, double distFromOrigin);

	DoubleVec3 GetNearestPoint(DoubleVec3 referencePoint) const;
	double GetAltitudeOfPoint(DoubleVec3 referencePoint) const;

};