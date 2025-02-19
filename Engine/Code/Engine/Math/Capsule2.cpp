#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/MathUtils.hpp"

Capsule2::Capsule2(Capsule2 const& copyfrom)
{
	m_start = copyfrom.m_start;
	m_end = copyfrom.m_end;
	m_radius = copyfrom.m_radius;

	m_length = GetAxis().GetLength();
}

Capsule2::Capsule2(Vec2 start, Vec2 end, float radius)
	:m_start(start), m_end(end), m_radius(radius)
{
	m_length = GetAxis().GetLength();
}

void Capsule2::FixLength()
{
	float deltaLength = GetAxis().GetLength();
	if (FloatEqual(m_length, deltaLength, 0.001f))
	{
		return;
	}
	float diff = (deltaLength - m_length) / deltaLength;
	m_start += GetAxis() * 0.5f * diff;
	m_end -= GetAxis() * 0.5f * diff;
}

Vec2 Capsule2::GetAxis() const
{
	return m_end - m_start;
}

float Capsule2::GetBoneLength() const
{
	return m_length;
}
