#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/MathUtils.hpp"

LineSegment2::LineSegment2(LineSegment2 const& copyfrom)
{
	m_start = copyfrom.m_start;
	m_end = copyfrom.m_end;
}

LineSegment2::LineSegment2(Vec2 start, Vec2 end)
	:m_start(start), m_end(end)
{
}
