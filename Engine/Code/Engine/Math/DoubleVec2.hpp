#pragma once
#include <math.h>

//-----------------------------------------------------------------------------------------------
struct DoubleVec2
{
public: // NOTE: this is one of the few cases where we break both the "m_" naming rule AND the avoid-public-members rule
	double x = 0.f;
	double y = 0.f;

public:
	// Construction/Destruction
	~DoubleVec2() {}												// destructor (do nothing)
	DoubleVec2() {}												// default constructor (do nothing)
	DoubleVec2( const DoubleVec2& copyFrom );							// copy constructor (from another DoubleVec2)
	explicit DoubleVec2( double initialX, double initialY );		// explicit constructor (from x, y)

	// Static methods
	static DoubleVec2 const MakeFromPolarRadians(double orientationRadians, double length = 1.f);
	static DoubleVec2 const MakeFromPolarDegrees(double orientationDegrees, double length = 1.f);

	static DoubleVec2 ZERO;
	static DoubleVec2 ONE;

	void SetFromText(char const* text);

	// Accessors (const methods)
	double		GetLength() const;
	double		GetLengthSquared() const;
	double		GetOrientationRadians() const;
	double		GetOrientationDegrees() const;
	DoubleVec2 const	GetRotated90Degrees() const;
	DoubleVec2 const	GetRotatedMinus90Degrees() const;
	DoubleVec2 const	GetRotatedRadians(double deltaRadians) const;
	DoubleVec2 const	GetRotatedDegrees(double deltaDegrees) const;
	DoubleVec2 const	GetClamped(double maxLength) const;
	DoubleVec2 const	GetNormalized() const;
	DoubleVec2 const	GetReflected(DoubleVec2 normalizedDoubleVec2) const;

	//Mutators
	void SetOrientationRadians(double newOrientationDegrees);
	void SetOrientationDegrees(double newOrientationRadians);
	void SetPolarRadians(double newOrientationRadians, double newLength);
	void SetPolarDegrees(double newOrientationDegrees, double newLength);
	void Rotate90Degrees();
	void RotateMinus90Degrees();
	void RotateRadians(double deltaRadians);
	void RotateDegrees(double deltaDegrees);
	void SetLength(double newLength);
	void ClampLength(double maxLength);
	void Normalize();
	double NormalizeAndGetPreviousLength();
	void Reflect(DoubleVec2 normalizedDoubleVec2);

	// Operators (const)
	bool		operator==( const DoubleVec2& compare ) const;		// DoubleVec2 == DoubleVec2
	bool		operator!=( const DoubleVec2& compare ) const;		// DoubleVec2 != DoubleVec2
	const DoubleVec2	operator+( const DoubleVec2& vecToAdd ) const;		// DoubleVec2 + DoubleVec2
	const DoubleVec2	operator-( const DoubleVec2& vecToSubtract ) const;	// DoubleVec2 - DoubleVec2
	const DoubleVec2	operator-() const;								// -DoubleVec2, i.e. "unary negation"
	const DoubleVec2	operator*( double uniformScale ) const;			// DoubleVec2 * double
	const DoubleVec2	operator*( const DoubleVec2& vecToMultiply ) const;	// DoubleVec2 * DoubleVec2
	const DoubleVec2	operator/( double inverseScale ) const;			// DoubleVec2 / double

	// Operators (self-mutating / non-const)
	void		operator+=( const DoubleVec2& vecToAdd );				// DoubleVec2 += DoubleVec2
	void		operator-=( const DoubleVec2& vecToSubtract );		// DoubleVec2 -= DoubleVec2
	void		operator*=( const double uniformScale );			// DoubleVec2 *= double
	void		operator/=( const double uniformDivisor );		// DoubleVec2 /= double
	void		operator=( const DoubleVec2& copyFrom );				// DoubleVec2 = DoubleVec2

	// Standalone "friend" functions that are conceptually, but not actually, part of DoubleVec2::
	friend const DoubleVec2 operator*( double uniformScale, const DoubleVec2& vecToScale );	// double * DoubleVec2
};


