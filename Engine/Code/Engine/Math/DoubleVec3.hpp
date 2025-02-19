#pragma once

struct Vec3;

//-----------------------------------------------------------------------------------------------
struct DoubleVec3
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	double x = 0.f;
	double y = 0.f;
	double z = 0.f;

	DoubleVec3() = default;
	DoubleVec3(Vec3 v);
	DoubleVec3(double initialX, double initialY, double initialZ);

	static DoubleVec3 ZERO;

	static DoubleVec3 const MakeFromPolarRadians(double latitudeRadians, double longitudeRadians, double length = 1.f);
	static DoubleVec3 const MakeFromPolarDegrees(double latitudeDegrees, double longitudeDegrees, double length = 1.f);

	static DoubleVec3 Lerp(DoubleVec3 const v1, DoubleVec3 const v2, double t);

	void SetFromText(char const* text);
	//Accessors
	double		GetLength() const;
	double		GetLengthXY() const;
	double		GetLengthSquared() const;
	double		GetLengthXYSquared() const;
	double		GetAngleAboutZRadians() const;
	double		GetAngleAboutZDegrees() const;
	DoubleVec3 const	GetRotatedAboutZRadians(double deltaRadians) const;
	DoubleVec3 const	GetRotatedAboutZDegrees(double deltaDegrees) const;
	DoubleVec3 const	GetClamped(double maxLength) const;
	DoubleVec3 const	GetNormalized() const;
	DoubleVec3 const	GetAbsolute() const;
	DoubleVec3			GetPerpendicularVectorAroundThisAxis(DoubleVec3 axis) const;
	bool		IsDifferent(DoubleVec3 compare, double tolerance = 0.f);
	double		GetAngleDegreeFromThisVector(const DoubleVec3& axis);
	void		LerpTo(DoubleVec3& goal, double t);
	void		UniformClamp(double min, double max);

	void		SetLength(double newLength);
	void		ClampLength(double maxLength);
	void		Normalize();
	double		Dot(DoubleVec3 v) const;
	DoubleVec3		Cross(DoubleVec3 v) const;
	// Operators (const)
	bool		operator==(const DoubleVec3& compare) const;
	bool		operator!=(const DoubleVec3& compare) const;
	const DoubleVec3	operator+(const DoubleVec3& vecToAdd) const;
	const DoubleVec3	operator-(const DoubleVec3& vecToSubtract) const;
	const DoubleVec3	operator-() const;
	const DoubleVec3	operator*(double uniformScale) const;
	const DoubleVec3	operator/(double inverseScale) const;
	const DoubleVec3	operator*(const DoubleVec3& vecToMultiply) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const DoubleVec3& vecToAdd);
	void		operator-=(const DoubleVec3& vecToSubtract);
	void		operator*=(const double uniformScale);
	void		operator/=(const double uniformDivisor);
	void		operator=(const DoubleVec3& copyFrom);

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec3::
	friend DoubleVec3 const operator*(double uniformScale, const DoubleVec3& vecToScale);
};


