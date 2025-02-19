#pragma once

struct Vec2;

struct IntVec2
{
public:
	int x = 0;
	int y = 0;

public:
	~IntVec2() = default;
	IntVec2() = default;
	IntVec2(const IntVec2& copyFrom);
	explicit IntVec2(Vec2& copyFrom);
	explicit IntVec2(int initialX, int initialY);

	static IntVec2 ZERO;

	static IntVec2 EAST;
	static IntVec2 NORTH;
	static IntVec2 WEST;
	static IntVec2 SOUTH;

	static IntVec2 NORTH_EAST;
	static IntVec2 NORTH_WEST;
	static IntVec2 SOUTH_EAST;
	static IntVec2 SOUTH_WEST;

	void SetFromText(char const* text);

	float GetLength() const;
	int GetTaxicabLength() const;
	int GetLengthSquared() const;
	float GetOrientationRadians() const;
	float GetOrientationDegrees() const;
	IntVec2 const GetRotated90Degrees() const;
	IntVec2 const GetRotatedMinus90Degrees() const;
	IntVec2 const GetNormalized() const;

	void Rotate90Degrees();
	void RotateMinus90Degrees();

	// Operators (const)
	bool		operator==(const IntVec2& compare) const;			
	bool		operator!=(const IntVec2& compare) const;			
	const IntVec2	operator+(const IntVec2& vecToAdd) const;		
	const IntVec2	operator-(const IntVec2& vecToSubtract) const;	
	const IntVec2	operator-() const;								
	const IntVec2	operator*(int uniformScale) const;			
	const IntVec2	operator*(const IntVec2& vecToMultiply) const;	
	const IntVec2	operator/(int inverseScale) const;
	const bool		operator<(const IntVec2& vecToCompare) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const IntVec2& vecToAdd);				
	void		operator-=(const IntVec2& vecToSubtract);			
	void		operator*=(const int uniformScale);
	void		operator/=(const int uniformDivisor);
	void		operator=(const IntVec2& copyFrom);					

	// Standalone "friend" functions that are conceptually, but not actually, part of Vec2::
	friend const IntVec2 operator*(int uniformScale, const IntVec2& vecToScale);
};