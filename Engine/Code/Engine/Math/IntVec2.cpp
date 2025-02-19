#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <math.h>

IntVec2::IntVec2(const IntVec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}

IntVec2::IntVec2(int initialX, int initialY)
{
	x = initialX;
	y = initialY;
}

IntVec2::IntVec2(Vec2& copyFrom)
{
	x = (int)copyFrom.x;
	y = (int)copyFrom.y;
}

IntVec2 IntVec2::ZERO = IntVec2(0, 0);

IntVec2 IntVec2::EAST = IntVec2(1, 0);
IntVec2 IntVec2::NORTH = IntVec2(0, 1);
IntVec2 IntVec2::WEST = IntVec2(-1, 0);
IntVec2 IntVec2::SOUTH = IntVec2(0, -1);

IntVec2 IntVec2::NORTH_EAST = IntVec2(1, 1);
IntVec2 IntVec2::NORTH_WEST = IntVec2(-1, 1);
IntVec2 IntVec2::SOUTH_EAST = IntVec2(1, -1);
IntVec2 IntVec2::SOUTH_WEST = IntVec2(-1, -1);

float IntVec2::GetLength() const
{
	return sqrtf((float)(x * x + y * y));
}

int IntVec2::GetTaxicabLength() const
{
	return abs(x) + abs(y);
}

int IntVec2::GetLengthSquared() const
{
	return x * x + y * y;
}

float IntVec2::GetOrientationRadians() const
{
	return atan2f((float)y, (float)x);
}

float IntVec2::GetOrientationDegrees() const
{
	return Atan2Degrees((float)y, (float)x);
}

IntVec2 const IntVec2::GetRotated90Degrees() const
{
	return IntVec2(-y, x);
}

IntVec2 const IntVec2::GetRotatedMinus90Degrees() const
{
	return IntVec2(y, -x);
}
IntVec2 const IntVec2::GetNormalized() const
{
	if (GetLength() == 0) return IntVec2(0, 0);
	float length = GetLength();
	float scale = 1.f / length;
	return IntVec2(int(x * scale), int(y * scale));
}

void IntVec2::Rotate90Degrees()
{
	IntVec2 temp = IntVec2(x, y);
	x = -temp.y;
	y = temp.x;
}

void IntVec2::RotateMinus90Degrees()
{
	IntVec2 temp = IntVec2(x, y);
	x = temp.y;
	y = -temp.x;
}

void IntVec2::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() != 2)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT INTVEC2");
	}

	const char* xChar = strings[0].c_str();
	const char* yChar = strings[1].c_str();

	x = atoi(xChar);
	y = atoi(yChar);
}

//-----------------------------------------------------------------------------------------------
const IntVec2 IntVec2::operator + (const IntVec2& vecToAdd) const
{
	return IntVec2(x + vecToAdd.x, y + vecToAdd.y);
}


//-----------------------------------------------------------------------------------------------
const IntVec2 IntVec2::operator-(const IntVec2& vecToSubtract) const
{
	return IntVec2(x - vecToSubtract.x, y - vecToSubtract.y);
}

//-----------------------------------------------------------------------------------------------
const IntVec2 IntVec2::operator-() const {
	return IntVec2(-x, -y);
}

//-----------------------------------------------------------------------------------------------
const IntVec2 IntVec2::operator*(int uniformScale) const
{
	return IntVec2(x * uniformScale, y * uniformScale);
}


//------------------------------------------------------------------------------------------------
const IntVec2 IntVec2::operator*(const IntVec2& vecToMultiply) const
{
	return IntVec2(x * vecToMultiply.x, y * vecToMultiply.y);
}


//-----------------------------------------------------------------------------------------------
const IntVec2 IntVec2::operator/(int inverseScale) const
{
	float scale = 1.f / inverseScale;
	return IntVec2((int)(x * scale), (int)(y * scale));
}


const bool IntVec2::operator<(const IntVec2& vecToCompare) const
{
	if (y > vecToCompare.y)
	{
		return false;
	}
	else if (y < vecToCompare.y)
	{
		return true;
	}
	else
	{
		return x < vecToCompare.x;
	}
}

//-----------------------------------------------------------------------------------------------
void IntVec2::operator+=(const IntVec2& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator-=(const IntVec2& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator*=(const int uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator/=(const int uniformDivisor)
{
	float scale = 1.f / uniformDivisor;
	x = (int)((float)x * scale);
	y = (int)((float)y * scale);
}


//-----------------------------------------------------------------------------------------------
void IntVec2::operator=(const IntVec2& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
}


//-----------------------------------------------------------------------------------------------
const IntVec2 operator*(int uniformScale, const IntVec2& vecToScale)
{
	int x = vecToScale.x * uniformScale;
	int y = vecToScale.y * uniformScale;
	return IntVec2(x, y);
}


//-----------------------------------------------------------------------------------------------
bool IntVec2::operator==(const IntVec2& compare) const
{
	return x == compare.x && y == compare.y;
}


//-----------------------------------------------------------------------------------------------
bool IntVec2::operator!=(const IntVec2& compare) const
{
	return x != compare.x || y != compare.y;
}
