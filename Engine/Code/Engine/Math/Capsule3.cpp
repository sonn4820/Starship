#include "Engine/Math/Capsule3.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/MathUtils.hpp"

Capsule3::Capsule3(Capsule3 const& copyfrom)
{
	m_start = copyfrom.m_start;
	m_end = copyfrom.m_end;
	m_radius = copyfrom.m_radius;

	m_length = GetAxisLength();
}

Capsule3::Capsule3(Vec3 start, Vec3 end, float radius)
	:m_start(start), m_end(end), m_radius(radius)
{
	m_length = GetAxisLength();
}

Capsule3::Capsule3(DoubleCapsule3 const& doubleCapsule)
{
	m_start = doubleCapsule.m_start;
	m_end = doubleCapsule.m_end;
	m_radius = static_cast<float>(doubleCapsule.m_radius);

	m_length = GetAxisLength();
}

Vec3 Capsule3::GetAxis() const
{
	return m_end - m_start;
}

Vec3 Capsule3::GetAxisNormal() const
{
	return (m_end - m_start).GetNormalized();
}

float Capsule3::GetAxisLength() const
{
	// get the current length
	return (m_end - m_start).GetLength();
}

float Capsule3::GetAxisHalfLength() const
{
	return (m_end - m_start).GetLength() * 0.5f;
}

Vec3 Capsule3::GetNearestPoint(Vec3 const point) const
{
	Vec3 NearestPointOnBone = GetNearestPointOnLineSegment3D(m_start, m_end, point);
	Vec3 FromNearestToPoint = (point - NearestPointOnBone);

	float length = Clamp(FromNearestToPoint.GetLength(), 0, m_radius);

	return NearestPointOnBone + FromNearestToPoint.GetNormalized() * length;
}

bool Capsule3::IsPointInside(Vec3 const point) const
{
	Vec3 NearestPointOnBone = GetNearestPointOnLineSegment3D(m_start, m_end, point);
	return 	IsPointInsideSphere3D(point, NearestPointOnBone, m_radius);
}

AABB3 Capsule3::GetBound() const
{
	Vec3 kBasis = GetAxisNormal();

	Vec3 tip = m_end + kBasis * m_radius;
	Vec3 base = m_start - kBasis * m_radius;

	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Vec3 max = tip - iBasis * m_radius - jBasis * m_radius + kBasis * m_radius;
	Vec3 min = base + iBasis * m_radius + jBasis * m_radius - kBasis * m_radius;

	return AABB3(min, max);
}

float Capsule3::GetBoneLength() const
{
	// get the og length
	return m_length;
}

void Capsule3::FixLength()
{
	float deltaLength = GetAxisLength();
	if (FloatEqual(m_length, deltaLength, 0.001f))
	{
		return;
	}
	float diff = (deltaLength - m_length) / deltaLength;
	m_start += GetAxis() * 0.5f * diff;
	m_end -= GetAxis() * 0.5f * diff;
}

LineSegment3 Capsule3::GetBone() const
{
	return LineSegment3(m_start, m_end);
}

Vec3 Capsule3::support(const Vec3& direction) const
{
	UNUSED(direction);
	return Vec3();
}
