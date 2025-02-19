#include "Engine/Math/IntVec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <math.h>

IntVec3::IntVec3(const IntVec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}

IntVec3::IntVec3(int initialX, int initialY, int initialZ)
{
	x = initialX;
	y = initialY;
	z = initialZ;
}

IntVec3 IntVec3::ZERO = IntVec3(0, 0, 0);

void IntVec3::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() != 3)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT INTVEC2");
	}

	const char* xChar = strings[0].c_str();
	const char* yChar = strings[1].c_str();
	const char* zChar = strings[2].c_str();

	x = atoi(xChar);
	y = atoi(yChar);
	z = atoi(zChar);
}

//-----------------------------------------------------------------------------------------------
const IntVec3 IntVec3::operator + (const IntVec3& vecToAdd) const
{
	return IntVec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}


//-----------------------------------------------------------------------------------------------
const IntVec3 IntVec3::operator-(const IntVec3& vecToSubtract) const
{
	return IntVec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

//-----------------------------------------------------------------------------------------------
const IntVec3 IntVec3::operator-() const {
	return IntVec3(-x, -y, -z);
}

//-----------------------------------------------------------------------------------------------
const IntVec3 IntVec3::operator*(int uniformScale) const
{
	return IntVec3(x * uniformScale, y * uniformScale, z * uniformScale);
}


//------------------------------------------------------------------------------------------------
const IntVec3 IntVec3::operator*(const IntVec3& vecToMultiply) const
{
	return IntVec3(x * vecToMultiply.x, y * vecToMultiply.y, z * vecToMultiply.z);
}


//-----------------------------------------------------------------------------------------------
const IntVec3 IntVec3::operator/(int inverseScale) const
{
	float scale = 1.f / inverseScale;
	return IntVec3((int)(x * scale), (int)(y * scale), (int)(z * scale));
}


//-----------------------------------------------------------------------------------------------
void IntVec3::operator+=(const IntVec3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}


//-----------------------------------------------------------------------------------------------
void IntVec3::operator-=(const IntVec3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}


//-----------------------------------------------------------------------------------------------
void IntVec3::operator*=(const int uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void IntVec3::operator/=(const int uniformDivisor)
{
	float scale = 1.f / uniformDivisor;
	x = (int)((float)x * scale);
	y = (int)((float)y * scale);
	z = (int)((float)z * scale);
}


//-----------------------------------------------------------------------------------------------
void IntVec3::operator=(const IntVec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}


//-----------------------------------------------------------------------------------------------
const IntVec3 operator*(int uniformScale, const IntVec3& vecToScale)
{
	int x = vecToScale.x * uniformScale;
	int y = vecToScale.y * uniformScale;
	int z = vecToScale.z * uniformScale;
	return IntVec3(x, y, z);
}


//-----------------------------------------------------------------------------------------------
bool IntVec3::operator==(const IntVec3& compare) const
{
	return x == compare.x && y == compare.y && z == compare.z;
}


//-----------------------------------------------------------------------------------------------
bool IntVec3::operator!=(const IntVec3& compare) const
{
	return x != compare.x || y != compare.y || z != compare.z;
}
