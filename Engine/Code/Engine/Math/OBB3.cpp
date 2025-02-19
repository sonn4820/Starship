#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

OBB3::OBB3(OBB3 const& copyfrom)
	:m_center(copyfrom.m_center), m_halfDimensions(copyfrom.m_halfDimensions), m_iBasisNormal(copyfrom.m_iBasisNormal), m_jBasisNormal(copyfrom.m_jBasisNormal)
{

}

OBB3::OBB3(Vec3 center, Vec3 iBasisNormal, Vec3 jBasisNormal, Vec3 halfDimensions)
	:m_center(center), m_halfDimensions(halfDimensions), m_iBasisNormal(iBasisNormal), m_jBasisNormal(jBasisNormal)
{

}

OBB3::OBB3(DoubleOBB3 const& doubleObb)
	:m_center(doubleObb.m_center), m_halfDimensions(doubleObb.m_halfDimensions), m_iBasisNormal(doubleObb.m_iBasisNormal), m_jBasisNormal(doubleObb.m_jBasisNormal)
{

}

bool OBB3::IsPointInside(Vec3 const& point) const
{
	Vec3 toPoint = point - m_center;
	Vec3 kBasisNormal = CrossProduct3D(m_iBasisNormal, m_jBasisNormal).GetNormalized();
	float pointI = DotProduct3D(toPoint, m_iBasisNormal);
	float pointJ = DotProduct3D(toPoint, m_jBasisNormal);
	float pointK = DotProduct3D(toPoint, kBasisNormal);
	if (pointI >= m_halfDimensions.x) return false;
	if (pointI <= -m_halfDimensions.x) return false;
	if (pointJ >= m_halfDimensions.y) return false;
	if (pointJ <= -m_halfDimensions.y) return false;
	if (pointK >= m_halfDimensions.z) return false;
	if (pointK <= -m_halfDimensions.z) return false;
	return true;
}

Vec3 const OBB3::GetNearestPoint(Vec3 const& referencePosition) const
{
	Vec3 kBasisNormal = CrossProduct3D(m_iBasisNormal, m_jBasisNormal).GetNormalized();
	Vec3 toPoint = referencePosition - m_center;

	float refPosI = DotProduct3D(toPoint, m_iBasisNormal);
	float refPosJ = DotProduct3D(toPoint, m_jBasisNormal);
	float refPosK = DotProduct3D(toPoint, kBasisNormal);

	float nearestPointI = Clamp(refPosI, -m_halfDimensions.x, m_halfDimensions.x);
	float nearestPointJ = Clamp(refPosJ, -m_halfDimensions.y, m_halfDimensions.y);
	float nearestPointK = Clamp(refPosK, -m_halfDimensions.z, m_halfDimensions.z);

	return m_center + (m_iBasisNormal * nearestPointI) + (m_jBasisNormal * nearestPointJ) + (kBasisNormal * nearestPointK);
}

void OBB3::GetCornerPoints(Vec3* out_fourCornerWorldPositions) const
{
	Vec3 kBasisNormal = CrossProduct3D(m_iBasisNormal, m_jBasisNormal).GetNormalized();

	Vec3 xExtent = m_halfDimensions.x * m_iBasisNormal;
	Vec3 yExtent = m_halfDimensions.y * m_jBasisNormal;
	Vec3 zExtent = m_halfDimensions.z * kBasisNormal;

	out_fourCornerWorldPositions[0] = m_center - xExtent + yExtent - zExtent;		 //BLNear
	out_fourCornerWorldPositions[1] = m_center - xExtent - yExtent - zExtent;		 //BRNear
	out_fourCornerWorldPositions[2] = m_center - xExtent + yExtent + zExtent;		 //TLNear
	out_fourCornerWorldPositions[3] = m_center - xExtent - yExtent + zExtent;		 //TRNear

	out_fourCornerWorldPositions[4] = m_center + xExtent + yExtent - zExtent;		 //BLFar
	out_fourCornerWorldPositions[5] = m_center + xExtent - yExtent - zExtent;		 //BRFar
	out_fourCornerWorldPositions[6] = m_center + xExtent + yExtent + zExtent;		 //TLFar
	out_fourCornerWorldPositions[7] = m_center + xExtent - yExtent + zExtent;		 //TRFar
}

Vec3 OBB3::support(const Vec3& direction) const
{
	UNUSED(direction);
	return Vec3();
}

