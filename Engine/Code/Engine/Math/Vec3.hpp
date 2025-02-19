#pragma once

struct DoubleVec3;
struct Vec2;

//-----------------------------------------------------------------------------------------------
struct Vec3
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	Vec3() = default;
	Vec3(float initialX, float initialY, float initialZ);
	Vec3(DoubleVec3 dv);
	Vec3(Vec2 v2, float v2z);

	static Vec3 ZERO;

	static Vec3 const MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length = 1.f);
	static Vec3 const MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length = 1.f);

	static Vec3 Lerp(Vec3 const v1, Vec3 const v2, float t);

	void SetFromText(char const* text);
	//Accessors
	float		GetLength() const;
	float		GetLengthXY() const;
	float		GetLengthSquared() const;
	float		GetLengthXYSquared() const;
	float		GetAngleAboutZRadians() const;
	float		GetAngleAboutZDegrees() const;
	Vec3 const	GetRotatedAboutZRadians(float deltaRadians) const;
	Vec3 const	GetRotatedAboutZDegrees(float deltaDegrees) const;
	Vec3 const	GetClamped(float maxLength) const;
	Vec3 const	GetNormalized() const;
	Vec3 const	GetPerpendicularVector() const;
	bool		IsDifferent(Vec3 compare, float tolerance = 0.f);
	float		GetAngleDegreeFromThisVector(const Vec3& axis);
	void		LerpTo(Vec3& goal, float t);

	void		SetLength(float newLength);
	void		ClampLength(float maxLength);
	void		Normalize();
	float		Dot(const Vec3& v);
	Vec3		Cross(const Vec3& v);
	// Operators (const)
	bool		operator==(const Vec3& compare) const;
	bool		operator!=(const Vec3& compare) const;
	const Vec3	operator+(const Vec3& vecToAdd) const;
	const Vec3	operator-(const Vec3& vecToSubtract) const;
	const Vec3	operator-() const;
	const Vec3	operator*(float uniformScale) const;
	const Vec3	operator/(float inverseScale) const;
	const Vec3	operator*(const Vec3& vecToMultiply) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const Vec3& vecToAdd);
	void		operator-=(const Vec3& vecToSubtract);
	void		operator*=(const float uniformScale);
	void		operator/=(const float uniformDivisor);
	void		operator=(const Vec3& copyFrom);

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec3::
	friend Vec3 const operator*(float uniformScale, const Vec3& vecToScale);
};


