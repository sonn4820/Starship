#pragma once
#include "Engine/Math/Vec3.hpp"

struct DoublePlane3;
struct Plane3
{
	Vec3 m_normal = Vec3(0.f, 0.f, 1.f);
	float m_distanceFromOrigin = 0.f;

	Plane3() = default;
	Plane3(const Plane3& copyfrom);
	Plane3(const DoublePlane3& doublePlane);
	explicit Plane3(Vec3 normal, float distFromOrigin);

	Vec3 GetNearestPoint(Vec3 referencePoint) const;
	float GetAltitudeOfPoint(Vec3 referencePoint) const;

};