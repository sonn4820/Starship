#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

Vec4::Vec4(float initialX, float initialY, float initialZ, float initialW)
	: x(initialX), y(initialY), z(initialZ), w(initialW)
{

}

Vec4::Vec4(Vec2 vec2, float initialZ, float initialW)
	: x(vec2.x), y(vec2.y), z(initialZ), w(initialW)
{

}

Vec4::Vec4(Vec3 vec3, float initialW)
	: x(vec3.x), y(vec3.y), z(vec3.z), w(initialW)
{

}

Vec4 Vec4::ZERO = Vec4(0, 0, 0, 0);

//-----------------------------------------------------------------------------------------------
bool Vec4::operator==(const Vec4& compare) const
{
	return x == compare.x && y == compare.y && z == compare.z && w == compare.w;
}


//-----------------------------------------------------------------------------------------------
bool Vec4::operator!=(const Vec4& compare) const
{
	return x != compare.x || y != compare.y || z != compare.z || w != compare.w;
}
//-----------------------------------------------------------------------------------------------
const Vec4 Vec4::operator+(const Vec4& vecToAdd) const
{
	return Vec4(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z, w + vecToAdd.w);
}


//-----------------------------------------------------------------------------------------------
const Vec4 Vec4::operator-(const Vec4& vecToSubtract) const
{
	return Vec4(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z, w - vecToSubtract.w);
}

//-----------------------------------------------------------------------------------------------
const Vec4	Vec4::operator*(float uniformScale) const
{
	return Vec4(x * uniformScale, y * uniformScale, z * uniformScale, w * uniformScale);
}

const Vec4 operator*(float uniformScale, const Vec4& vecToScale)
{
	float x = vecToScale.x * uniformScale;
	float y = vecToScale.y * uniformScale;
	float z = vecToScale.z * uniformScale;
	float w = vecToScale.w * uniformScale;
	return Vec4(x, y, z, w);
}

//-----------------------------------------------------------------------------------------------
const Vec4 Vec4::operator/(float inverseScale) const
{
	float scale = 1.f / inverseScale;
	return Vec4(x * scale, y * scale, z * scale, w * scale);
}

//-----------------------------------------------------------------------------------------------
void Vec4::operator+=(const Vec4& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w += vecToAdd.w;
}

//-----------------------------------------------------------------------------------------------
void Vec4::operator-=(const Vec4& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
	w -= vecToSubtract.w;
}

//-----------------------------------------------------------------------------------------------
void Vec4::operator*=(float uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
	w *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
void Vec4::operator/=(float uniformDivisor)
{
	float scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

//-----------------------------------------------------------------------------------------------
void Vec4::operator=(const Vec4& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}
