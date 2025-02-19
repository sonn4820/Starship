#include "Engine/Math/DoubleVec4.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

DoubleVec4::DoubleVec4(double initialX, double initialY, double initialZ, double initialW)
	: x(initialX), y(initialY), z(initialZ), w(initialW)
{

}

DoubleVec4::DoubleVec4(DoubleVec2 DoubleVec2, double initialZ, double initialW)
	: x(DoubleVec2.x), y(DoubleVec2.y), z(initialZ), w(initialW)
{

}

DoubleVec4::DoubleVec4(DoubleVec3 DoubleVec3, double initialW)
	: x(DoubleVec3.x), y(DoubleVec3.y), z(DoubleVec3.z), w(initialW)
{

}

DoubleVec4 DoubleVec4::ZERO = DoubleVec4(0, 0, 0, 0);

//-----------------------------------------------------------------------------------------------
bool DoubleVec4::operator==(const DoubleVec4& compare) const
{
	return x == compare.x && y == compare.y && z == compare.z && w == compare.w;
}


//-----------------------------------------------------------------------------------------------
bool DoubleVec4::operator!=(const DoubleVec4& compare) const
{
	return x != compare.x || y != compare.y || z != compare.z || w != compare.w;
}
//-----------------------------------------------------------------------------------------------
const DoubleVec4 DoubleVec4::operator+(const DoubleVec4& vecToAdd) const
{
	return DoubleVec4(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z, w + vecToAdd.w);
}


//-----------------------------------------------------------------------------------------------
const DoubleVec4 DoubleVec4::operator-(const DoubleVec4& vecToSubtract) const
{
	return DoubleVec4(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z, w - vecToSubtract.w);
}

//-----------------------------------------------------------------------------------------------
const DoubleVec4	DoubleVec4::operator*(double uniformScale) const
{
	return DoubleVec4(x * uniformScale, y * uniformScale, z * uniformScale, w * uniformScale);
}

const DoubleVec4 operator*(double uniformScale, const DoubleVec4& vecToScale)
{
	double x = vecToScale.x * uniformScale;
	double y = vecToScale.y * uniformScale;
	double z = vecToScale.z * uniformScale;
	double w = vecToScale.w * uniformScale;
	return DoubleVec4(x, y, z, w);
}

//-----------------------------------------------------------------------------------------------
const DoubleVec4 DoubleVec4::operator/(double inverseScale) const
{
	double scale = 1.f / inverseScale;
	return DoubleVec4(x * scale, y * scale, z * scale, w * scale);
}

//-----------------------------------------------------------------------------------------------
void DoubleVec4::operator+=(const DoubleVec4& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
	w += vecToAdd.w;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec4::operator-=(const DoubleVec4& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
	w -= vecToSubtract.w;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec4::operator*=(double uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
	w *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec4::operator/=(double uniformDivisor)
{
	double scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec4::operator=(const DoubleVec4& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
	w = copyFrom.w;
}
