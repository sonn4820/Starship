#include "Engine/Math/DoublePlane3.hpp"
#include "Engine/Math/MathUtils.hpp"

DoublePlane3::DoublePlane3(DoublePlane3 const& copyfrom)
{
	m_normal = copyfrom.m_normal;
	m_distanceFromOrigin = copyfrom.m_distanceFromOrigin;
}

DoublePlane3::DoublePlane3(DoubleVec3 normal, double distFromOrigin)
	:m_normal(normal), m_distanceFromOrigin(distFromOrigin)
{

}

DoublePlane3::DoublePlane3(const Plane3& plane)
{
	m_normal = plane.m_normal;
	m_distanceFromOrigin = plane.m_distanceFromOrigin;
}

DoubleVec3 DoublePlane3::GetNearestPoint(DoubleVec3 referencePoint) const
{
	double refPosAltitude = GetAltitudeOfPoint(referencePoint);
	return referencePoint + m_normal * -refPosAltitude;
}

double DoublePlane3::GetAltitudeOfPoint(DoubleVec3 referencePoint) const
{
	return DotProduct3D_Double(referencePoint, m_normal) - m_distanceFromOrigin;
}
