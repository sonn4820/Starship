#pragma once
#include "Engine/Math/Vec3.hpp"
struct LineSegment3
{

public:
	Vec3 m_start;
	Vec3 m_end;
	   
public:

	~LineSegment3() = default;
	LineSegment3() = default;
	LineSegment3(LineSegment3 const& copyfrom);
	explicit LineSegment3(Vec3 start, Vec3 end);

	Vec3 GetAxis() const;
};