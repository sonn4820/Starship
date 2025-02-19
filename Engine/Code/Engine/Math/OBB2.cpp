#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

OBB2::OBB2(OBB2 const& copyfrom)
{
	m_center = copyfrom.m_center;
	m_halfDimensions = copyfrom.m_halfDimensions;
	m_iBasisNormal = copyfrom.m_iBasisNormal;
}

OBB2::OBB2(Vec2 center, Vec2 iBasisNormal, Vec2 halfDimensions)
	:m_center(center), m_iBasisNormal(iBasisNormal), m_halfDimensions(halfDimensions)
{
}

OBB2::OBB2(Vec2 center, float angle, Vec2 halfDimensions)
	:m_center(center), m_iBasisNormal(Vec2::MakeFromPolarDegrees(angle)), m_halfDimensions(halfDimensions)
{
	
}

bool OBB2::IsPointInside(Vec2 const& point) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();
	Vec2 vectorToPointFromCenter = point - m_center;
	float pointI = DotProduct2D(vectorToPointFromCenter, m_iBasisNormal);
	float pointJ = DotProduct2D(vectorToPointFromCenter, jBasisNormal);
	if (pointI >= m_halfDimensions.x) return false;
	if (pointI <= -m_halfDimensions.x) return false;
	if (pointJ >= m_halfDimensions.y) return false;
	if (pointJ <= -m_halfDimensions.y) return false;
	return true;
}

Vec2 const OBB2::GetNearestPoint(Vec2 const& referencePosition) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();
	Vec2 vectorToRefPosFromCenter = referencePosition - m_center;
	float refPosI = DotProduct2D(vectorToRefPosFromCenter, m_iBasisNormal);
	float refPosJ = DotProduct2D(vectorToRefPosFromCenter, jBasisNormal);

	float nearestPointI = Clamp(refPosI, -m_halfDimensions.x, m_halfDimensions.x);
	float nearestPointJ = Clamp(refPosJ, -m_halfDimensions.y, m_halfDimensions.y);

	return m_center + (m_iBasisNormal * nearestPointI) + (jBasisNormal * nearestPointJ);
}

void OBB2::GetCornerPoints(Vec2* out_fourCornerWorldPositions) const
{
	Vec2 jBasisNormal = m_iBasisNormal.GetRotated90Degrees();
	Vec2 xExtent = m_halfDimensions.x * m_iBasisNormal;
	Vec2 yExtent = m_halfDimensions.y * jBasisNormal;

	out_fourCornerWorldPositions[0] = m_center + xExtent + yExtent;
	out_fourCornerWorldPositions[1] = m_center - xExtent + yExtent;
	out_fourCornerWorldPositions[2] = m_center - xExtent - yExtent;
	out_fourCornerWorldPositions[3] = m_center + xExtent - yExtent;
}

Vec2 OBB2::GetLocalPosForWorldPos(Vec2 worldPos) const
{
	return worldPos + m_center.x * m_iBasisNormal + m_center.y * m_iBasisNormal.GetRotated90Degrees();
}
