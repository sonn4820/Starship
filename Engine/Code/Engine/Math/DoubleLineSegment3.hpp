#pragma once
#include "Engine/Math/DoubleVec3.hpp"
struct DoubleLineSegment3
{

public:
	DoubleVec3 m_start;
	DoubleVec3 m_end;
	   
public:

	~DoubleLineSegment3() = default;
	DoubleLineSegment3() = default;
	DoubleLineSegment3(DoubleLineSegment3 const& copyfrom);
	explicit DoubleLineSegment3(DoubleVec3 start, DoubleVec3 end);

	DoubleVec3 GetAxis() const;
};