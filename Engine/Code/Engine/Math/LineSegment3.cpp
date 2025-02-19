#include "Engine/Math/LineSegment3.hpp"

LineSegment3::LineSegment3(LineSegment3 const& copyfrom)
{
	m_start = copyfrom.m_start;
	m_end = copyfrom.m_end;
}

LineSegment3::LineSegment3(Vec3 start, Vec3 end)
	:m_start(start), m_end(end)
{
}

Vec3 LineSegment3::GetAxis() const
{
	return m_end - m_start;
}
