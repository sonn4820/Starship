#include "Engine/Math/DoubleOBB3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

DoubleOBB3::DoubleOBB3(DoubleOBB3 const& copyfrom)
	:m_center(copyfrom.m_center), m_halfDimensions(copyfrom.m_halfDimensions), m_iBasisNormal(copyfrom.m_iBasisNormal), m_jBasisNormal(copyfrom.m_jBasisNormal)
{

}

DoubleOBB3::DoubleOBB3(DoubleVec3 center, DoubleVec3 iBasisNormal, DoubleVec3 jBasisNormal, DoubleVec3 halfDimensions)
	:m_center(center), m_halfDimensions(halfDimensions), m_iBasisNormal(iBasisNormal), m_jBasisNormal(jBasisNormal)
{

}

DoubleOBB3::DoubleOBB3(OBB3 const& obb)
	:m_center(obb.m_center), m_halfDimensions(obb.m_halfDimensions), m_iBasisNormal(obb.m_iBasisNormal), m_jBasisNormal(obb.m_jBasisNormal)
{

}

bool DoubleOBB3::IsPointInside(DoubleVec3 const& point) const
{
	DoubleVec3 toPoint = point - m_center;
	DoubleVec3 kBasisNormal = CrossProduct3D_Double(m_iBasisNormal, m_jBasisNormal).GetNormalized();
	double pointI = DotProduct3D_Double(toPoint, m_iBasisNormal);
	double pointJ = DotProduct3D_Double(toPoint, m_jBasisNormal);
	double pointK = DotProduct3D_Double(toPoint, kBasisNormal);
	if (pointI >= m_halfDimensions.x) return false;
	if (pointI <= -m_halfDimensions.x) return false;
	if (pointJ >= m_halfDimensions.y) return false;
	if (pointJ <= -m_halfDimensions.y) return false;
	if (pointK >= m_halfDimensions.z) return false;
	if (pointK <= -m_halfDimensions.z) return false;
	return true;
}

DoubleVec3 const DoubleOBB3::GetNearestPoint(DoubleVec3 const& referencePosition) const
{
	DoubleVec3 kBasisNormal = CrossProduct3D_Double(m_iBasisNormal, m_jBasisNormal).GetNormalized();
	DoubleVec3 toPoint = referencePosition - m_center;

	double refPosI = DotProduct3D_Double(toPoint, m_iBasisNormal);
	double refPosJ = DotProduct3D_Double(toPoint, m_jBasisNormal);
	double refPosK = DotProduct3D_Double(toPoint, kBasisNormal);

	double nearestPointI = Clamp_Double(refPosI, -m_halfDimensions.x, m_halfDimensions.x);
	double nearestPointJ = Clamp_Double(refPosJ, -m_halfDimensions.y, m_halfDimensions.y);
	double nearestPointK = Clamp_Double(refPosK, -m_halfDimensions.z, m_halfDimensions.z);

	return m_center + (m_iBasisNormal * nearestPointI) + (m_jBasisNormal * nearestPointJ) + (kBasisNormal * nearestPointK);
}

void DoubleOBB3::GetCornerPoints(DoubleVec3* out_fourCornerWorldPositions) const
{
	DoubleVec3 kBasisNormal = CrossProduct3D_Double(m_iBasisNormal, m_jBasisNormal).GetNormalized();

	DoubleVec3 xExtent = m_halfDimensions.x * m_iBasisNormal;
	DoubleVec3 yExtent = m_halfDimensions.y * m_jBasisNormal;
	DoubleVec3 zExtent = m_halfDimensions.z * kBasisNormal;

	out_fourCornerWorldPositions[0] = m_center - xExtent + yExtent - zExtent;		 //BLNear
	out_fourCornerWorldPositions[1] = m_center - xExtent - yExtent - zExtent;		 //BRNear
	out_fourCornerWorldPositions[2] = m_center - xExtent + yExtent + zExtent;		 //TLNear
	out_fourCornerWorldPositions[3] = m_center - xExtent - yExtent + zExtent;		 //TRNear

	out_fourCornerWorldPositions[4] = m_center + xExtent + yExtent - zExtent;		 //BLFar
	out_fourCornerWorldPositions[5] = m_center + xExtent - yExtent - zExtent;		 //BRFar
	out_fourCornerWorldPositions[6] = m_center + xExtent + yExtent + zExtent;		 //TLFar
	out_fourCornerWorldPositions[7] = m_center + xExtent - yExtent + zExtent;		 //TRFar
}

DoubleAABB3 DoubleOBB3::GetBoundingBox() const
{
	DoubleVec3 kBasis = m_iBasisNormal.Cross(m_jBasisNormal).GetNormalized();

	DoubleVec3 i = m_iBasisNormal * m_halfDimensions.x;
	DoubleVec3 j = m_jBasisNormal * m_halfDimensions.y;
	DoubleVec3 k = kBasis * m_halfDimensions.z;

	double ex = fabs(i.x) + fabs(j.x) + fabs(k.x);
	double ey = fabs(i.y) + fabs(j.y) + fabs(k.y);
	double ez = fabs(i.z) + fabs(j.z) + fabs(k.z);

	DoubleVec3 min(m_center.x - ex, m_center.y - ey, m_center.z - ez);
	DoubleVec3 max(m_center.x + ex, m_center.y + ey, m_center.z + ez);

	return DoubleAABB3(min, max);
}
