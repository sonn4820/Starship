#pragma once


//-----------------------------------------------------------------------------------------------
struct DoubleVec2;
struct DoubleVec3;

struct DoubleVec4
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	double x = 0.f;
	double y = 0.f;
	double z = 0.f;
	double w = 0.f;

	DoubleVec4() = default;
	DoubleVec4(double initialX, double initialY, double initialZ, double initialW);
	DoubleVec4(DoubleVec2 DoubleVec2, double initialZ = 0.f, double initialW = 1.f);
	DoubleVec4(DoubleVec3 DoubleVec3, double initialW = 1.f);

	static DoubleVec4 ZERO;

	// Operators (const)
	bool		operator==(const DoubleVec4& compare) const;
	bool		operator!=(const DoubleVec4& compare) const;
	const DoubleVec4	operator+(const DoubleVec4& vecToAdd) const;
	const DoubleVec4	operator-(const DoubleVec4& vecToSubtract) const;
	const DoubleVec4	operator*(double uniformScale) const;
	const DoubleVec4	operator/(double inverseScale) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const DoubleVec4& vecToAdd);
	void		operator-=(const DoubleVec4& vecToSubtract);
	void		operator*=(const double uniformScale);
	void		operator/=(const double uniformDivisor);
	void		operator=(const DoubleVec4& copyFrom);

	// Standalone "friend" functions that are conceptually, but not actually, part of DoubleVec4::
	friend DoubleVec4 const operator*(double uniformScale, const DoubleVec4& vecToScale);
};


