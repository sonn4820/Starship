#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/MathUtils.hpp"

Plane3::Plane3(Plane3 const& copyfrom)
{
	m_normal = copyfrom.m_normal;
	m_distanceFromOrigin = copyfrom.m_distanceFromOrigin;
}

Plane3::Plane3(Vec3 normal, float distFromOrigin)
	:m_normal(normal), m_distanceFromOrigin(distFromOrigin)
{

}

Plane3::Plane3(const DoublePlane3& doublePlane)
{
	m_normal = doublePlane.m_normal;
	m_distanceFromOrigin = static_cast<float>(doublePlane.m_distanceFromOrigin);
}

Vec3 Plane3::GetNearestPoint(Vec3 referencePoint) const
{
	float refPosAltitude = GetAltitudeOfPoint(referencePoint);
	return referencePoint + m_normal * -refPosAltitude;
}

float Plane3::GetAltitudeOfPoint(Vec3 referencePoint) const
{
	return DotProduct3D(referencePoint, m_normal) - m_distanceFromOrigin;
}
