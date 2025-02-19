#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/MathUtils.hpp"

Plane2::Plane2(Plane2 const& copyfrom)
{
	m_normal = copyfrom.m_normal;
	m_distanceFromOrigin = copyfrom.m_distanceFromOrigin;
}

Plane2::Plane2(Vec2 normal, float distFromOrigin)
	:m_normal(normal), m_distanceFromOrigin(distFromOrigin)
{

}


Vec2 Plane2::GetNearestPoint(Vec2 referencePoint) const
{
	float refPosAltitude = GetAltitudeOfPoint(referencePoint);
	return referencePoint + m_normal * -refPosAltitude;
}

float Plane2::GetAltitudeOfPoint(Vec2 referencePoint) const
{
	return DotProduct2D(referencePoint, m_normal) - m_distanceFromOrigin;
}

void Plane2::Translate(Vec2 translation)
{
	float movedDist = DotProduct2D(translation, m_normal);
	m_distanceFromOrigin += movedDist;
}
