#include "Engine/Math/DoubleCapsule3.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/MathUtils.hpp"

DoubleCapsule3::DoubleCapsule3(DoubleCapsule3 const& copyfrom)
{
	m_start = copyfrom.m_start;
	m_end = copyfrom.m_end;
	m_radius = copyfrom.m_radius;

	m_length = GetAxisLength();
}

DoubleCapsule3::DoubleCapsule3(DoubleVec3 start, DoubleVec3 end, double radius)
	:m_start(start), m_end(end), m_radius(radius)
{
	m_length = GetAxisLength();
}

DoubleCapsule3::DoubleCapsule3(Capsule3 const& capsule)
{
	m_start = capsule.m_start;
	m_end = capsule.m_end;
	m_radius = capsule.m_radius;

	m_length = GetAxisLength();
}

DoubleVec3 DoubleCapsule3::GetAxis() const
{
	return m_end - m_start;
}

DoubleVec3 DoubleCapsule3::GetAxisNormal() const
{
	return (m_end - m_start).GetNormalized();
}

double DoubleCapsule3::GetAxisLength() const
{
	// get the current length
	return (m_end - m_start).GetLength();
}

double DoubleCapsule3::GetAxisHalfLength() const
{
	return (m_end - m_start).GetLength() * 0.5;
}

DoubleVec3 DoubleCapsule3::GetNearestPoint(DoubleVec3 const point) const
{
	DoubleVec3 NearestPointOnBone = GetNearestPointOnLineSegment3D(m_start, m_end, point);
	DoubleVec3 FromNearestToPoint = (point - NearestPointOnBone);

	double length = Clamp_Double(FromNearestToPoint.GetLength(), 0, m_radius);

	return NearestPointOnBone + FromNearestToPoint.GetNormalized() * length;
}

bool DoubleCapsule3::IsPointInside(DoubleVec3 const point) const
{
	DoubleVec3 NearestPointOnBone = GetNearestPointOnLineSegment3D(m_start, m_end, point);
	return 	IsPointInsideSphere3D_Double(point, NearestPointOnBone, m_radius);
}

DoubleAABB3 DoubleCapsule3::GetBound() const
{
	DoubleVec3 kBasis = GetAxisNormal();

	DoubleVec3 tip = m_end + kBasis * m_radius;
	DoubleVec3 base = m_start - kBasis * m_radius;

	DoubleVec3 iBasis;
	DoubleVec3 jBasis;

	if (fabs(DotProduct3D(kBasis, DoubleVec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(DoubleVec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, DoubleVec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	DoubleVec3 max = tip - iBasis * m_radius - jBasis * m_radius + kBasis * m_radius;
	DoubleVec3 min = base + iBasis * m_radius + jBasis * m_radius - kBasis * m_radius;

	return DoubleAABB3(min, max);
}

double DoubleCapsule3::GetBoneLength() const
{
	// get the og length
	return m_length;
}

void DoubleCapsule3::FixLength()
{
	double deltaLength = GetAxisLength();
	if (DoubleEqual(m_length, deltaLength, 0.001f))
	{
		return;
	}
	double diff = (deltaLength - m_length) / deltaLength;
	m_start += GetAxis() * 0.5f * diff;
	m_end -= GetAxis() * 0.5f * diff;
}

DoubleLineSegment3 DoubleCapsule3::GetBone() const
{
	return DoubleLineSegment3(m_start, m_end);
}

DoubleVec3 DoubleCapsule3::GetCenter() const
{
	return m_start + GetAxisNormal() * GetAxisLength() * 0.5f;
}

DoubleOBB3 DoubleCapsule3::GetOrientedBox() const
{
	DoubleVec3 axis = GetAxis();

	DoubleVec3 center = m_start + axis * 0.5;
	DoubleVec3 iBasis = axis.GetNormalized();

	DoubleVec3 kBasis;
	DoubleVec3 jBasis;

	if (fabs(DotProduct3D_Double(iBasis, DoubleVec3(0, 0, 1.0))) < 0.9999)
	{
		jBasis = CrossProduct3D_Double(DoubleVec3(0, 0, 1), iBasis).GetNormalized();
		kBasis = CrossProduct3D_Double(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D_Double(iBasis, DoubleVec3(0, 1, 0)).GetNormalized();
		jBasis = CrossProduct3D_Double(kBasis, iBasis);
	}

	DoubleVec3 halfDimension((axis.GetLength() + m_radius * 2) * 0.5, m_radius, m_radius);

	DoubleOBB3 obb;
	obb.m_center = center;
	obb.m_halfDimensions = halfDimension;
	obb.m_iBasisNormal = iBasis;
	obb.m_jBasisNormal = jBasis;

	return obb;
}

DoubleAABB3 DoubleCapsule3::GetBoundingBox() const
{
	return GetOrientedBox().GetBoundingBox();
}

