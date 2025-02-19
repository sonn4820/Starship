#include "Engine/Math/DoubleLineSegment3.hpp"

DoubleLineSegment3::DoubleLineSegment3(DoubleLineSegment3 const& copyfrom)
{
	m_start = copyfrom.m_start;
	m_end = copyfrom.m_end;
}

DoubleLineSegment3::DoubleLineSegment3(DoubleVec3 start, DoubleVec3 end)
	:m_start(start), m_end(end)
{
}

DoubleVec3 DoubleLineSegment3::GetAxis() const
{
	return m_end - m_start;
}
