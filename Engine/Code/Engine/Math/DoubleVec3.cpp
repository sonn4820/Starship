#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <math.h>

DoubleVec3::DoubleVec3(double initialX, double initialY, double initialZ)
	: x(initialX), y(initialY), z(initialZ)
{

}

DoubleVec3::DoubleVec3(Vec3 v)
{
	x = static_cast<double>(v.x);
	y = static_cast<double>(v.y);
	z = static_cast<double>(v.z);
}

DoubleVec3 const DoubleVec3::MakeFromPolarRadians(double latitudeRadians, double longitudeRadians, double length)
{
	DoubleVec3 v;
	v.x = length * cos(latitudeRadians) * cos(longitudeRadians);
	v.y = length * cos(latitudeRadians) * sin(longitudeRadians);
	v.z = length * -sin(latitudeRadians);
	return v;
}

DoubleVec3 const DoubleVec3::MakeFromPolarDegrees(double latitudeDegrees, double longitudeDegrees, double length)
{
	DoubleVec3 v;
	v.x = length * CosDegreesDouble(latitudeDegrees) * CosDegreesDouble(longitudeDegrees);
	v.y = length * CosDegreesDouble(latitudeDegrees) * SinDegreesDouble(longitudeDegrees);
	v.z = length * -SinDegreesDouble(latitudeDegrees);
	return v;
}

DoubleVec3 DoubleVec3::Lerp(DoubleVec3 const v1, DoubleVec3 const v2, double t)
{
	return Interpolate(v1, v2, t);
}

DoubleVec3 DoubleVec3::ZERO = DoubleVec3(0.f, 0.f, 0.f);

void DoubleVec3::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() != 3)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT Vec3_DP");
	}
	const char* xChar = strings[0].c_str();
	const char* yChar = strings[1].c_str();
	const char* zChar = strings[2].c_str();

	x = atof(xChar);
	y = atof(yChar);
	z = atof(zChar);
}

double DoubleVec3::GetLength() const
{
	return sqrt((x * x) + (y * y) + (z * z));
}
double DoubleVec3::GetLengthXY() const
{
	return sqrt((x * x) + (y * y));
}
double DoubleVec3::GetLengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}
double DoubleVec3::GetLengthXYSquared() const
{
	return (x * x) + (y * y);
}
double DoubleVec3::GetAngleAboutZRadians() const
{
	return atan2(y, x);
}
double DoubleVec3::GetAngleAboutZDegrees() const
{
	return Atan2DegreesDouble(y, x);
}
DoubleVec3 const DoubleVec3::GetRotatedAboutZRadians(double deltaRadians) const
{
	// Find the new angle, then change x and y based on the new angle, keep z the same
	double newThetaAngle = GetAngleAboutZRadians() + deltaRadians;
	double newX = cos(newThetaAngle) * GetLengthXY();
	double newY = sin(newThetaAngle) * GetLengthXY();
	return DoubleVec3(newX, newY, z);
}
DoubleVec3 const DoubleVec3::GetRotatedAboutZDegrees(double deltaDegrees) const
{
	// Find the new angle, then change x and y based on the new angle, keep z the same
	double newThetaAngle = GetAngleAboutZDegrees() + deltaDegrees;
	double newX = CosDegreesDouble(newThetaAngle) * GetLengthXY();
	double newY = SinDegreesDouble(newThetaAngle) * GetLengthXY();
	return DoubleVec3(newX, newY, z);
}
DoubleVec3 const DoubleVec3::GetClamped(double maxLength) const
{
	double length = GetLength();
	if (length <= 0.f) return DoubleVec3(0.f, 0.f, 0.f);
	if (length >= maxLength) return DoubleVec3(x, y, z);
	double scale = maxLength / length;
	return DoubleVec3(x * scale, y * scale, z * scale);
}
DoubleVec3 const DoubleVec3::GetNormalized() const
{
	double length = GetLength();
	if (length <= 0.f) return DoubleVec3(0.f, 0.f, 0.f);
	double scale = 1 / length;
	return DoubleVec3(x * scale, y * scale, z * scale);
}

DoubleVec3 const DoubleVec3::GetAbsolute() const
{
	return DoubleVec3(fabs(x), fabs(y), fabs(z));
}

DoubleVec3 DoubleVec3::GetPerpendicularVectorAroundThisAxis(DoubleVec3 axis) const
{
	DoubleVec3 v = *this;
	DoubleVec3 normalizedAxis = axis;
	normalizedAxis.Normalize();

	// Check if the vectors are not parallel
	if (fabs(v.x * normalizedAxis.x + v.y * normalizedAxis.y + v.z * normalizedAxis.z) < 0.99999) {
		// Use cross product to find a perpendicular vector
		DoubleVec3 perpendicular = v.Cross(normalizedAxis);
		perpendicular.Normalize();
		return perpendicular;
	}
	else {
		// The vectors are parallel, so we need to choose an arbitrary perpendicular vector
		DoubleVec3 arbitrary(1, 0, 0);
		if (std::abs(normalizedAxis.x) > 0.99999) {
			arbitrary = DoubleVec3(0, 1, 0);
		}
		DoubleVec3 perpendicular = normalizedAxis.Cross(arbitrary);
		perpendicular.Normalize();
		return perpendicular;
	}
}

bool DoubleVec3::IsDifferent(DoubleVec3 compare, double tolerance)
{
	DoubleRange xRange = DoubleRange(x - tolerance, x + tolerance);
	DoubleRange yRange = DoubleRange(y - tolerance, y + tolerance);
	DoubleRange zRange = DoubleRange(z - tolerance, z + tolerance);

	if (xRange.IsOnRange(compare.x) && yRange.IsOnRange(compare.y) && zRange.IsOnRange(compare.z))
	{
		return false;
	}
	else
	{
		return true;
	}
}

double DoubleVec3::GetAngleDegreeFromThisVector(const DoubleVec3& axis)
{
	double dot = this->Dot(axis);

	double angle = ConvertRadiansToDegreesDouble(acos(dot/ (GetLength() * axis.GetLength())));

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

void DoubleVec3::LerpTo(DoubleVec3& goal, double t)
{
	Interpolate(*this, goal, t);
}

void DoubleVec3::UniformClamp(double min, double max)
{
	x = Clamp_Double(x, min, max);
	y = Clamp_Double(y, min, max);
	z = Clamp_Double(z, min, max);
}

void DoubleVec3::SetLength(double newLength)
{
	Normalize();
	*this *= newLength;
}

void DoubleVec3::ClampLength(double maxLength)
{
	double length = GetLength();
	if (length <= 0) return;
	if (length <= maxLength) return;
	double scale = maxLength / length;
	x *= scale;
	y *= scale;
	z *= scale;
}

void DoubleVec3::Normalize()
{
	double length = GetLength();
	if (length <= 0.f)
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	else
	{
		double scale = 1 / length;
		x *= scale;
		y *= scale;
		z *= scale;
	}
}

double DoubleVec3::Dot(DoubleVec3 v) const
{
	return DotProduct3D_Double(*this, v);
}

DoubleVec3 DoubleVec3::Cross(DoubleVec3 v) const
{
	return CrossProduct3D_Double(*this, v);
}

//-----------------------------------------------------------------------------------------------
bool DoubleVec3::operator==(const DoubleVec3& compare) const
{
	return 	DoubleEqual(x, compare.x, 0.00001f) 
		&&	DoubleEqual(y, compare.y, 0.00001f) 
		&&	DoubleEqual(z, compare.z, 0.00001f);
}


//-----------------------------------------------------------------------------------------------
bool DoubleVec3::operator!=(const DoubleVec3& compare) const
{
	return x != compare.x || y != compare.y || z != compare.z;
}
//-----------------------------------------------------------------------------------------------
const DoubleVec3 DoubleVec3::operator+(const DoubleVec3& vecToAdd) const
{
	return DoubleVec3(x + vecToAdd.x, y + vecToAdd.y, z + vecToAdd.z);
}


//-----------------------------------------------------------------------------------------------
const DoubleVec3 DoubleVec3::operator-(const DoubleVec3& vecToSubtract) const
{
	return DoubleVec3(x - vecToSubtract.x, y - vecToSubtract.y, z - vecToSubtract.z);
}

const DoubleVec3 DoubleVec3::operator-() const
{
 	return DoubleVec3(-x, -y, -z);
}

//-----------------------------------------------------------------------------------------------
const DoubleVec3	DoubleVec3::operator*(double uniformScale) const
{
	return DoubleVec3(x * uniformScale, y * uniformScale, z * uniformScale);
}

const DoubleVec3 DoubleVec3::operator*(const DoubleVec3& vecToMultiply) const
{
	double newx = x * vecToMultiply.x;
	double newy = y * vecToMultiply.y;
	double newz = z * vecToMultiply.z;
	return DoubleVec3(newx, newy, newz);
}

const DoubleVec3 operator*(double uniformScale, const DoubleVec3& vecToScale)
{
	double x = vecToScale.x * uniformScale;
	double y = vecToScale.y * uniformScale;
	double z = vecToScale.z * uniformScale;
	return DoubleVec3(x, y, z);
}

//-----------------------------------------------------------------------------------------------
const DoubleVec3 DoubleVec3::operator/(double inverseScale) const
{
	double scale = 1.f / inverseScale;
	return DoubleVec3(x * scale, y * scale, z * scale);
}

//-----------------------------------------------------------------------------------------------
void DoubleVec3::operator+=(const DoubleVec3& vecToAdd)
{
	x += vecToAdd.x;
	y += vecToAdd.y;
	z += vecToAdd.z;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec3::operator-=(const DoubleVec3& vecToSubtract)
{
	x -= vecToSubtract.x;
	y -= vecToSubtract.y;
	z -= vecToSubtract.z;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec3::operator*=(double uniformScale)
{
	x *= uniformScale;
	y *= uniformScale;
	z *= uniformScale;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec3::operator/=(double uniformDivisor)
{
	double scale = 1.f / uniformDivisor;
	x *= scale;
	y *= scale;
	z *= scale;
}

//-----------------------------------------------------------------------------------------------
void DoubleVec3::operator=(const DoubleVec3& copyFrom)
{
	x = copyFrom.x;
	y = copyFrom.y;
	z = copyFrom.z;
}
