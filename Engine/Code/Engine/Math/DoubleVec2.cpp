#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/DoubleVec2.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <math.h>


//-----------------------------------------------------------------------------------------------
DoubleVec2::DoubleVec2(const DoubleVec2& copy) : x(copy.x), y(copy.y)
{
}


//-----------------------------------------------------------------------------------------------
DoubleVec2::DoubleVec2(double initialX, double initialY) : x(initialX), y(initialY)
{
}
DoubleVec2 DoubleVec2::ZERO = DoubleVec2(0, 0);
DoubleVec2 DoubleVec2::ONE = DoubleVec2(1, 1);
//-----------------------------------------------------------------------------------------------
// Static methods
DoubleVec2 const DoubleVec2::MakeFromPolarRadians(double orientationRadians, double length)
{
	// Find x and y based on the angle
	double newX = cos(orientationRadians) * length;
	double newY = sin(orientationRadians) * length;
	return DoubleVec2(newX, newY);
}
DoubleVec2 const DoubleVec2::MakeFromPolarDegrees(double orientationDegrees, double length)
{
	// Find x and y based on the angle
	double newX = CosDegreesDouble(orientationDegrees) * length;
	double newY = SinDegreesDouble(orientationDegrees) * length;
	return DoubleVec2(newX, newY);
}

void DoubleVec2::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() != 2)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT DoubleVec2");
	}
	const char* xChar = strings[0].c_str();
	const char* yChar = strings[1].c_str();

	x = atof(xChar);
	y = atof(yChar);
}

//-----------------------------------------------------------------------------------------------
// Accessors (const methods)
double DoubleVec2::GetLength() const
{
	return sqrt((x * x) + (y * y));
}
double DoubleVec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}
double DoubleVec2::GetOrientationRadians() const
{
	return atan2(y, x);
}
double DoubleVec2::GetOrientationDegrees() const
{
	return Atan2DegreesDouble(y, x);
}
DoubleVec2 const DoubleVec2::GetRotated90Degrees() const
{
	// Switch x and y, then add - operator to define which quarter to rotate
	return DoubleVec2(-y, x);
}
DoubleVec2 const DoubleVec2::GetRotatedMinus90Degrees() const
{
	// Switch x and y, then add - operator to define which quarter to rotate
	return DoubleVec2(y, -x);
}
DoubleVec2 const DoubleVec2::GetRotatedRadians(double deltaRadians) const
{
	return MakeFromPolarRadians(GetOrientationRadians() + deltaRadians, GetLength());
}
DoubleVec2 const DoubleVec2::GetRotatedDegrees(double deltaDegrees) const
{
	return MakeFromPolarDegrees(GetOrientationDegrees() + deltaDegrees, GetLength());
}
DoubleVec2 const DoubleVec2::GetClamped(double maxLength) const
{
	double length = GetLength();
	if (length <= 0) return DoubleVec2(0, 0);
	if (length <= maxLength) return DoubleVec2(x, y);
	double scale = maxLength / length;
	return DoubleVec2(x * scale, y * scale);
}
DoubleVec2 const DoubleVec2::GetNormalized() const
{
	double length = GetLength();
	if (length <= 0) return DoubleVec2(0, 0);
	double scale = 1.f / length;
	return DoubleVec2(x * scale, y * scale);
}

DoubleVec2 const DoubleVec2::GetReflected(DoubleVec2 normalizedDoubleVec2) const
{
	double nLength = DotProduct2D_Double(DoubleVec2(x, y), normalizedDoubleVec2);
	DoubleVec2 n = normalizedDoubleVec2 * nLength;
	DoubleVec2 t = DoubleVec2(x, y) - n;
	return t - n;
}

//-----------------------------------------------------------------------------------------------
//Mutators
void DoubleVec2::SetOrientationRadians(double newOrientationRadians)
{
	// Define x and y based on the new angle
	double length = GetLength();
	x = cos(newOrientationRadians) * length;
	y = sin(newOrientationRadians) * length;
}
void DoubleVec2::SetOrientationDegrees(double newOrientationDegrees)
{
	// Define x and y based on the new angle
	double length = GetLength();
	x = CosDegreesDouble(newOrientationDegrees) * length;
	y = SinDegreesDouble(newOrientationDegrees) * length;
}
void DoubleVec2::SetPolarRadians(double newOrientationRadians, double newLength)
{
	// Define x and y based on new angle and new length
	x = cos(newOrientationRadians) * newLength;
	y = sin(newOrientationRadians) * newLength;
}
void DoubleVec2::SetPolarDegrees(double newOrientationDegrees, double newLength)
{
	// Define x and y based on new angle and new length
	x = CosDegreesDouble(newOrientationDegrees) * newLength;
	y = SinDegreesDouble(newOrientationDegrees) * newLength;
}
void DoubleVec2::Rotate90Degrees()
{
	// Switch x and y, then add - operator to define which quarter to rotate
	double tempX = x;
	double tempY = y;
	x = -tempY;
	y = tempX;
}
void DoubleVec2::RotateMinus90Degrees()
{
	// Switch x and y, then add - operator to define which quarter to rotate
	double tempX = x;
	double tempY = y;
	x = tempY;
	y = -tempX;
}
void DoubleVec2::RotateRadians(double deltaRadians)
{
	// Find the new angle, then define x and y based on the new angle
	double length = GetLength();
	double newThetaRadians = GetOrientationRadians() + deltaRadians;
	x = cos(newThetaRadians) * length;
	y = sin(newThetaRadians) * length;
}
void DoubleVec2::RotateDegrees(double deltaDegrees)
{
	// Find the new angle, then define x and y based on the new angle
	double length = GetLength();
	double newThetaRadians = GetOrientationDegrees() + deltaDegrees;
	x = CosDegreesDouble(newThetaRadians) * length;
	y = SinDegreesDouble(newThetaRadians) * length;
}
void DoubleVec2::SetLength(double newLength)
{
	// Define x and y based on the new length
	double length = GetLength();
	if (length == 0)
	{
		x = 0;
		y = 0;
		return;
	}
	double ratio = newLength / length;
	x *= ratio;
	y *= ratio;
}
void DoubleVec2::ClampLength(double maxLength)
{
	double length = GetLength();
	// Define x and y based on the max length
	if (length <= 0) return;
	if (length >= maxLength) return;
	double scale = maxLength / length;
	x *= scale;
	y *= scale;
}
void DoubleVec2::Normalize()
{
	// Length of vec will be 1, keep the same direction
	double length = GetLength();
	if (length > 0.f) {
		double scale = 1.f / length;
		x *= scale;
		y *= scale;
	}
}
double DoubleVec2::NormalizeAndGetPreviousLength()
{
	// Length of vec will be 1, keep the same direction, and return the old length
	double previousLength = GetLength();
	Normalize();
	return previousLength;
}

void DoubleVec2::Reflect(DoubleVec2 normalizedDoubleVec2)
{
	double normalDoubleVec2Length = DotProduct2D_Double(DoubleVec2(x, y), normalizedDoubleVec2);
	DoubleVec2 normalDoubleVec2 = normalizedDoubleVec2 * normalDoubleVec2Length;
	DoubleVec2 transitionDoubleVec2 = DoubleVec2(x, y) - normalDoubleVec2;
	DoubleVec2 transitionReflectedDoubleVec2 = transitionDoubleVec2;
	DoubleVec2 normalReflectedDoubleVec2 = -normalDoubleVec2;
	DoubleVec2 reflectedDoubleVec2 = transitionReflectedDoubleVec2 + normalReflectedDoubleVec2;
	x = reflectedDoubleVec2.x;
	y = reflectedDoubleVec2.y;
}

//-----------------------------------------------------------------------------------------------
const DoubleVec2 DoubleVec2::operator + (const DoubleVec2& vecToAdd) const
{
	return DoubleVec2(x + vecToAdd.x, y + vecToAdd.y);
}


//-----------------------------------------------------------------------------------------------
const DoubleVec2 DoubleVec2::operator-(const DoubleVec2& vecToSubtract) const
{
	return DoubleVec2(x - vecToSubtract.x, y - vecToSubtract.y);
}

//-----------------------------------------------------------------------------------------------
const DoubleVec2 DoubleVec2::operator-() const {
	return DoubleVec2(-x, -y);
}

//-----------------------------------------------------------------------------------------------
const DoubleVec2 DoubleVec2::operator*(double uniformScale) const
{
	return DoubleVec2(x * uniformScale, y * uniformScale);
}


//------------------------------------------------------------------------------------------------
const DoubleVec2 DoubleVec2::operator*(const DoubleVec2& vecToMultiply) const
{
	return DoubleVec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
const DoubleVec2 DoubleVec2::operator/(double inverseScale) const
{
	double scale = 1.f / inverseScale;
	return DoubleVec2(x * scale, y * scale);
}


//-----------------------------------------------------------------------------------------------
void DoubleVec2::operator+=(const DoubleVec2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void DoubleVec2::operator-=(const DoubleVec2& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void DoubleVec2::operator*=(const double uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void DoubleVec2::operator/=(const double uniformDivisor)
{
	double scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
}


//-----------------------------------------------------------------------------------------------
void DoubleVec2::operator=(const DoubleVec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//-----------------------------------------------------------------------------------------------
const DoubleVec2 operator*(double uniformScale, const DoubleVec2& vecToScale)
{
	double x = vecToScale.x * uniformScale;
	double y = vecToScale.y * uniformScale;
	return DoubleVec2(x, y);
}


//-----------------------------------------------------------------------------------------------
bool DoubleVec2::operator==(const DoubleVec2& compare) const
{
	return x == compare.x && y == compare.y;
}


//-----------------------------------------------------------------------------------------------
bool DoubleVec2::operator!=(const DoubleVec2& compare) const
{
	return x != compare.x || y != compare.y;
}


