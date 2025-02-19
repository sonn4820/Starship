#pragma once
#include "Engine/Math/Vec2.hpp"
struct LineSegment2
{

public:
	Vec2 m_start;
	Vec2 m_end;

public:

	~LineSegment2() = default;
	LineSegment2() = default;
	LineSegment2(LineSegment2 const& copyfrom);
	explicit LineSegment2(Vec2 start, Vec2 end);
};