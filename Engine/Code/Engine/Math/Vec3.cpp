#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <math.h>

Vec3::Vec3(float initialX, float initialY, float initialZ)
	: x(initialX), y(initialY), z(initialZ)
{

}

Vec3::Vec3(DoubleVec3 dv)
{
	x = static_cast<float>(dv.x);
	y = static_cast<float>(dv.y);
	z = static_cast<float>(dv.z);
}

Vec3::Vec3(Vec2 v2, float v2z)
{
	x = v2.x;
	y = v2.y;
	z = v2z;
}

Vec3 const Vec3::MakeFromPolarRadians(float latitudeRadians, float longitudeRadians, float length)
{
	Vec3 v;
	v.x = length * cosf(latitudeRadians) * cosf(longitudeRadians);
	v.y = length * cosf(latitudeRadians) * sinf(longitudeRadians);
	v.z = length * -sinf(latitudeRadians);
	return v;
}

Vec3 const Vec3::MakeFromPolarDegrees(float latitudeDegrees, float longitudeDegrees, float length)
{
	Vec3 v;
	v.x = length * CosDegrees(latitudeDegrees) * CosDegrees(longitudeDegrees);
	v.y = length * CosDegrees(latitudeDegrees) * SinDegrees(longitudeDegrees);
	v.z = length * -SinDegrees(latitudeDegrees);
	return v;
}

Vec3 Vec3::Lerp(Vec3 const v1, Vec3 const v2, float t)
{
	return Interpolate(v1, v2, t);
}

Vec3 Vec3::ZERO = Vec3(0.f, 0.f, 0.f);

void Vec3::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() != 3)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT VEC3");
	}
	const char* xChar = strings[0].c_str();
	const char* yChar = strings[1].c_str();
	const char* zChar = strings[2].c_str();

	x = static_cast<float>(atof(xChar));
	y = static_cast<float>(atof(yChar));
	z = static_cast<float>(atof(zChar));
}

float Vec3::GetLength() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}
float Vec3::GetLengthXY() const
{
	return sqrtf((x * x) + (y * y));
}
float Vec3::GetLengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}
float Vec3::GetLengthXYSquared() const
{
	return (x * x) + (y * y);
}
float Vec3::GetAngleAboutZRadians() const
{
	return atan2f(y, x);
}
float Vec3::GetAngleAboutZDegrees() const
{
	return Atan2Degrees(y, x);
}
Vec3 const Vec3::GetRotatedAboutZRadians(float deltaRadians) const
{
	// Find the new angle, then change x and y based on the new angle, keep z the same
	float newThetaAngle = GetAngleAboutZRadians() + deltaRadians;
	float newX = cosf(newThetaAngle) * GetLengthXY();
	float newY = sinf(newThetaAngle) * GetLengthXY();
	return Vec3(newX, newY, z);
}
Vec3 const Vec3::GetRotatedAboutZDegrees(float deltaDegrees) const
{
	// Find the new angle, then change x and y based on the new angle, keep z the same
	float newThetaAngle = GetAngleAboutZDegrees() + deltaDegrees;
	float newX = CosDegrees(newThetaAngle) * GetLengthXY();
	float newY = SinDegrees(newThetaAngle) * GetLengthXY();
	return Vec3(newX, newY, z);
}
Vec3 const Vec3::GetClamped(float maxLength) const
{
	float length = GetLength();
	if (length <= 0.f) return Vec3(0.f, 0.f, 0.f);
	if (length >= maxLength) return Vec3(x, y, z);
	float scale = maxLength / length;
	return Vec3(x * scale, y * scale, z * scale);
}
Vec3 const Vec3::GetNormalized() const
{
	float length = GetLength();
	if (length <= 0.f) return Vec3(0.f, 0.f, 0.f);
	float scale = 1 / length;
	return Vec3(x * scale, y * scale, z * scale);
}

Vec3 const Vec3::GetPerpendicularVector() const
{
	Vec3 perpVector;
	if (fabsf(x) < fabsf(y) && fabsf(x) < fabsf(z))
	{
		perpVector = Vec3(1, 0, 0);
	}
	else if (fabsf(y) < fabsf(x) && fabsf(y) < fabsf(z))
	{
		perpVector = Vec3(0, 1, 0);
	}
	else
	{
		perpVector = Vec3(0, 0, 1);
	}
	return CrossProduct3D(*this, perpVector).GetNormalized();
}

bool Vec3::IsDifferent(Vec3 compare, float tolerance)
{
	FloatRange xRange = FloatRange(x - tolerance, x + tolerance);
	FloatRange yRange = FloatRange(y - tolerance, y + tolerance);
	FloatRange zRange = FloatRange(z - tolerance, z + tolerance);

	if (xRange.IsOnRange(compare.x) && yRange.IsOnRange(compare.y) && zRange.IsOnRange(compare.z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

float Vec3::GetAngleDegreeFromThisVector(const Vec3& axis)
{
	float dot = this->Dot(axis);

	float angle = ConvertRadiansToDegrees(acosf(dot / (GetLength() * axis.GetLength())));

	if (angle < 0)
	{
		angle += 360.f;
	}
	if (angle > 360.f)
	{
		angle -= 360.f;
	}
	return angle;
}

void Vec3::LerpTo(Vec3& goal, float t)
{
	Interpolate(*this, goal, t);
}

void Vec3::SetLength(float newLength)
{
	float ratio = newLength / GetLength();
	x *= ratio;
	y *= ratio;
	z *= ratio;
}

void Vec3::ClampLength(float maxLength)
{
	float length = GetLength();
	if (length <= 0) return;
	if (length <= maxLength) return;
	float scale = maxLength / length;
	x *= scale;
	y *= scale;
	z *= scale;
}

void Vec3::Normalize()
{
	float length = GetLength();
	if (length <= 0.f)
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	else
	{
		float scale = 1 / length;
		x *= scale;
		y *= scale;
		z *= scale;
	}
}

float Vec3::Dot(const Vec3& v)
{
	return DotProduct3D(*this, v);
}

Vec3 Vec3::Cross(const Vec3& v)
{
	return CrossProduct3D(*this, v);
}

//-----------------------------------------------------------------------------------------------
bool Vec3::operator==(const Vec3& compare) const
{
	return 	FloatEqual(x, compare.x, 0.00001f)
		&& FloatEqual(y, compare.y, 0.00001f)
		&& FloatEqual(z, compare.z, 0.00001f);
}


//-----------------------------------------------------------------------------------------------
bool Vec3::operator!=(const Vec3& compare) const
{
	return x != compare.x || y != compare.y || z != compare.z;
}
//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator+(const Vec3& vecToAdd) const
{
	return Vec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}


//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator-(const Vec3& vecToSubtract) const
{
	return Vec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

const Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

//-----------------------------------------------------------------------------------------------
const Vec3	Vec3::operator*(float uniformScale) const
{
	return Vec3(x * uniformScale, y * uniformScale, z * uniformScale);
}

const Vec3 Vec3::operator*(const Vec3& vecToMultiply) const
{
	float newx = x * vecToMultiply.x;
	float newy = y * vecToMultiply.y;
	float newz = z * vecToMultiply.z;
	return Vec3(newx, newy, newz);
}

const Vec3 operator*(float uniformScale, const Vec3& vecToScale)
{
	float x = vecToScale.x * uniformScale;
	float y = vecToScale.y * uniformScale;
	float z = vecToScale.z * uniformScale;
	return Vec3(x, y, z);
}

//-----------------------------------------------------------------------------------------------
const Vec3 Vec3::operator/(float inverseScale) const
{
	float scale = 1.f / inverseScale;
	return Vec3(x * scale, y * scale, z * scale);
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator+=(const Vec3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator-=(const Vec3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator*=(float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator/=(float uniformDivisor)
{
	float scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
	z *= scale;
}

//-----------------------------------------------------------------------------------------------
void Vec3::operator=(const Vec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}
