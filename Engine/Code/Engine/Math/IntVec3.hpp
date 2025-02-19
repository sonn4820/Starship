#pragma once
struct IntVec3
{
public:
	int x = 0;
	int y = 0;
	int z = 0;

public:
	~IntVec3() = default;
	IntVec3() = default;
	IntVec3(const IntVec3& copyFrom);
	explicit IntVec3(int initialX, int initialY, int initialZ);

	static IntVec3 ZERO;

	void SetFromText(char const* text);

	// Operators (const)
	bool		operator==(const IntVec3& compare) const;
	bool		operator!=(const IntVec3& compare) const;
	const IntVec3	operator+(const IntVec3& vecToAdd) const;
	const IntVec3	operator-(const IntVec3& vecToSubtract) const;
	const IntVec3	operator-() const;
	const IntVec3	operator*(int uniformScale) const;
	const IntVec3	operator*(const IntVec3& vecToMultiply) const;
	const IntVec3	operator/(int inverseScale) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const IntVec3& vecToAdd);
	void		operator-=(const IntVec3& vecToSubtract);
	void		operator*=(const int uniformScale);
	void		operator/=(const int uniformDivisor);
	void		operator=(const IntVec3& copyFrom);

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec2::
	friend const IntVec3 operator*(int uniformScale, const IntVec3& vecToScale);
};