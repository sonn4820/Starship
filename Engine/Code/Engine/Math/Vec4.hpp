#pragma once


//-----------------------------------------------------------------------------------------------
struct Vec2;
struct Vec3;

struct Vec4
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

	Vec4() = default;
	Vec4(float initialX, float initialY, float initialZ, float initialW);
	Vec4(Vec2 vec2, float initialZ = 0.f, float initialW = 1.f);
	Vec4(Vec3 vec3, float initialW = 1.f);

	static Vec4 ZERO;

	// Operators (const)
	bool		operator==(const Vec4& compare) const;
	bool		operator!=(const Vec4& compare) const;
	const Vec4	operator+(const Vec4& vecToAdd) const;
	const Vec4	operator-(const Vec4& vecToSubtract) const;
	const Vec4	operator*(float uniformScale) const;
	const Vec4	operator/(float inverseScale) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const Vec4& vecToAdd);
	void		operator-=(const Vec4& vecToSubtract);
	void		operator*=(const float uniformScale);
	void		operator/=(const float uniformDivisor);
	void		operator=(const Vec4& copyFrom);

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec4::
	friend Vec4 const operator*(float uniformScale, const Vec4& vecToScale);
};


