#pragma once
#include "Engine/Math/Vec2.hpp"
struct Capsule2
{

public:
	Vec2 m_start;
	Vec2 m_end;
	float m_radius;

public:

	~Capsule2() = default;
	Capsule2() = default;
	Capsule2(Capsule2 const& copyfrom);
	explicit Capsule2(Vec2 start, Vec2 end, float radius);

	void FixLength();
	Vec2 GetAxis() const;
	float GetBoneLength() const;

private:
	float m_length;

};