#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <math.h>


//-----------------------------------------------------------------------------------------------
Vec2::Vec2(const Vec2& copy) : x(copy.x), y(copy.y)
{
}


//-----------------------------------------------------------------------------------------------
Vec2::Vec2(float initialX, float initialY) : x(initialX), y(initialY)
{
}

Vec2::Vec2(const IntVec2& copyFrom)
{
	x = (float)copyFrom.x;
	y = (float)copyFrom.y;
}

Vec2::Vec2(const Vec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

Vec2 Vec2::ZERO = Vec2(0, 0);
Vec2 Vec2::ONE = Vec2(1, 1);
//-----------------------------------------------------------------------------------------------
// Static methods
Vec2 const Vec2::MakeFromPolarRadians(float orientationRadians, float length)
{
	// Find x and y based on the angle
	float newX = cosf(orientationRadians) * length;
	float newY = sinf(orientationRadians) * length;
	return Vec2(newX, newY);
}
Vec2 const Vec2::MakeFromPolarDegrees(float orientationDegrees, float length)
{
	// Find x and y based on the angle
	float newX = CosDegrees(orientationDegrees) * length;
	float newY = SinDegrees(orientationDegrees) * length;
	return Vec2(newX, newY);
}

void Vec2::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() != 2)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT VEC2");
	}
	const char* xChar = strings[0].c_str();
	const char* yChar = strings[1].c_str();

	x = static_cast<float>(atof(xChar));
	y = static_cast<float>(atof(yChar));
}

//-----------------------------------------------------------------------------------------------
// Accessors (const methods)
float Vec2::GetLength() const
{
	return sqrtf((x * x) + (y * y));
}
float Vec2::GetLengthSquared() const
{
	return (x * x) + (y * y);
}
float Vec2::GetOrientationRadians() const
{
	return atan2f(y, x);
}
float Vec2::GetOrientationDegrees() const
{
	return Atan2Degrees(y, x);
}
Vec2 const Vec2::GetRotated90Degrees() const
{
	// Switch x and y, then add - operator to define which quarter to rotate
	return Vec2(-y, x);
}
Vec2 const Vec2::GetRotatedMinus90Degrees() const
{
	// Switch x and y, then add - operator to define which quarter to rotate
	return Vec2(y, -x);
}
Vec2 const Vec2::GetRotatedRadians(float deltaRadians) const
{
	return MakeFromPolarRadians(GetOrientationRadians() + deltaRadians, GetLength());
}
Vec2 const Vec2::GetRotatedDegrees(float deltaDegrees) const
{
	return MakeFromPolarDegrees(GetOrientationDegrees() + deltaDegrees, GetLength());
}
Vec2 const Vec2::GetClamped(float maxLength) const
{
	float length = GetLength();
	if (length <= 0) return Vec2(0, 0);
	if (length <= maxLength) return Vec2(x, y);
	float scale = maxLength / length;
	return Vec2(x * scale, y * scale);
}
Vec2 const Vec2::GetNormalized() const
{
	float length = GetLength();
	if (length <= 0) return Vec2(0, 0);
	float scale = 1.f / length;
	return Vec2(x * scale, y * scale);
}

Vec2 const Vec2::GetReflected(Vec2 normalizedVec2) const
{
	float nLength = DotProduct2D(Vec2(x, y), normalizedVec2);
	Vec2 n = normalizedVec2 * nLength;
	Vec2 t = Vec2(x, y) - n;
	return t - n;
}

float const Vec2::Dot(Vec2 v)
{
	return DotProduct2D(*this, v);
}

float const Vec2::Cross(Vec2 v)
{
	return CrossProduct2D(*this, v);
}

//-----------------------------------------------------------------------------------------------
//Mutators
void Vec2::SetOrientationRadians(float newOrientationRadians)
{
	// Define x and y based on the new angle
	float length = GetLength();
	x = cosf(newOrientationRadians) * length;
	y = sinf(newOrientationRadians) * length;
}
void Vec2::SetOrientationDegrees(float newOrientationDegrees)
{
	// Define x and y based on the new angle
	float length = GetLength();
	x = CosDegrees(newOrientationDegrees) * length;
	y = SinDegrees(newOrientationDegrees) * length;
}
void Vec2::SetPolarRadians(float newOrientationRadians, float newLength)
{
	// Define x and y based on new angle and new length
	x = cosf(newOrientationRadians) * newLength;
	y = sinf(newOrientationRadians) * newLength;
}
void Vec2::SetPolarDegrees(float newOrientationDegrees, float newLength)
{
	// Define x and y based on new angle and new length
	x = CosDegrees(newOrientationDegrees) * newLength;
	y = SinDegrees(newOrientationDegrees) * newLength;
}
void Vec2::Rotate90Degrees()
{
	// Switch x and y, then add - operator to define which quarter to rotate
	float tempX = x;
	float tempY = y;
	x = -tempY;
	y = tempX;
}
void Vec2::RotateMinus90Degrees()
{
	// Switch x and y, then add - operator to define which quarter to rotate
	float tempX = x;
	float tempY = y;
	x = tempY;
	y = -tempX;
}
void Vec2::RotateRadians(float deltaRadians)
{
	// Find the new angle, then define x and y based on the new angle
	float length = GetLength();
	float newThetaRadians = GetOrientationRadians() + deltaRadians;
	x = cosf(newThetaRadians) * length;
	y = sinf(newThetaRadians) * length;
}
void Vec2::RotateDegrees(float deltaDegrees)
{
	// Find the new angle, then define x and y based on the new angle
	float length = GetLength();
	float newThetaRadians = GetOrientationDegrees() + deltaDegrees;
	x = CosDegrees(newThetaRadians) * length;
	y = SinDegrees(newThetaRadians) * length;
}
void Vec2::SetLength(float newLength)
{
	// Define x and y based on the new length
	float length = GetLength();
	if (length == 0)
	{
		x = 0;
		y = 0;
		return;
	}
	float ratio = newLength / length;
	x *= ratio;
	y *= ratio;
}
void Vec2::ClampLength(float maxLength)
{
	float length = GetLength();
	// Define x and y based on the max length
	if (length <= 0) return;
	if (length >= maxLength) return;
	float scale = maxLength / length;
	x *= scale;
	y *= scale;
}
void Vec2::Normalize()
{
	// Length of vec will be 1, keep the same direction
	float length = GetLength();
	if (length > 0.f) {
		float scale = 1.f / length;
		x *= scale;
		y *= scale;
	}
}
float Vec2::NormalizeAndGetPreviousLength()
{
	// Length of vec will be 1, keep the same direction, and return the old length
	float previousLength = GetLength();
	Normalize();
	return previousLength;
}

void Vec2::Reflect(Vec2 normalizedVec2)
{
	float normalVec2Length = DotProduct2D(Vec2(x, y), normalizedVec2);
	Vec2 normalVec2 = normalizedVec2 * normalVec2Length;
	Vec2 transitionVec2 = Vec2(x, y) - normalVec2;
	Vec2 transitionReflectedVec2 = transitionVec2;
	Vec2 normalReflectedVec2 = -normalVec2;
	Vec2 reflectedVec2 = transitionReflectedVec2 + normalReflectedVec2;
	x = reflectedVec2.x;
	y = reflectedVec2.y;
}

float Vec2::GetAngleAboutZDegrees() const
{
	return Atan2Degrees(y, x);
}

//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator + (const Vec2& vecToAdd) const
{
	return Vec2(x + vecToAdd.x, y + vecToAdd.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-(const Vec2& vecToSubtract) const
{
	return Vec2(x - vecToSubtract.x, y - vecToSubtract.y);
}

//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator-() const {
	return Vec2(-x, -y);
}

//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*(float uniformScale) const
{
	return Vec2(x * uniformScale, y * uniformScale);
}


//------------------------------------------------------------------------------------------------
const Vec2 Vec2::operator*(const Vec2& vecToMultiply) const
{
	return Vec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
const Vec2 Vec2::operator/(float inverseScale) const
{
	float scale = 1.f / inverseScale;
	return Vec2(x * scale, y * scale);
}


const Vec2 Vec2::operator/(const Vec2& vecToDivide) const
{
	return Vec2(x / vecToDivide.x, y / vecToDivide.y);
}

//-----------------------------------------------------------------------------------------------
void Vec2::operator+=(const Vec2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator-=(const Vec2& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator*=(const float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator/=(const float uniformDivisor)
{
	float scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
}


//-----------------------------------------------------------------------------------------------
void Vec2::operator=(const Vec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//-----------------------------------------------------------------------------------------------
const Vec2 operator*(float uniformScale, const Vec2& vecToScale)
{
	float x = vecToScale.x * uniformScale;
	float y = vecToScale.y * uniformScale;
	return Vec2(x, y);
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator==(const Vec2& compare) const
{
	return x == compare.x && y == compare.y;
}


//-----------------------------------------------------------------------------------------------
bool Vec2::operator!=(const Vec2& compare) const
{
	return x != compare.x || y != compare.y;
}


