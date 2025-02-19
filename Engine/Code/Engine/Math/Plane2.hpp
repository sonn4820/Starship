#pragma once
#include "Engine/Math/Vec2.hpp"


struct Plane2
{
	Vec2 m_normal = Vec2(0.f, 1.f);
	float m_distanceFromOrigin = 0.f;

	Plane2() = default;
	Plane2(const Plane2& copyfrom);

	explicit Plane2(Vec2 normal, float distFromOrigin);

	Vec2 GetNearestPoint(Vec2 referencePoint) const;
	float GetAltitudeOfPoint(Vec2 referencePoint) const;
	void Translate(Vec2 translation);
};