#include "Engine/Math/DoubleQuaternion.hpp"
#include "Engine/Math/MathUtils.hpp"

DoubleQuaternion::DoubleQuaternion(double i, double j, double k)
	:i(i), j(j), k(k)
{

}

DoubleQuaternion::DoubleQuaternion(double i, double j, double k, double w)
	:i(i), j(j), k(k), w(w)
{

}

DoubleQuaternion::DoubleQuaternion(EulerAngles const& eulerAngle)
{
	double angle;

	angle = eulerAngle.m_rollDegrees * 0.5f;
	const double sr = SinDegreesDouble(angle);
	const double cr = CosDegreesDouble(angle);

	angle = eulerAngle.m_pitchDegrees * 0.5f;
	const double sp = SinDegreesDouble(angle);
	const double cp = CosDegreesDouble(angle);

	angle = eulerAngle.m_yawDegrees * 0.5f;
	const double sy = SinDegreesDouble(angle);
	const double cy = CosDegreesDouble(angle);

	const double cpcy = cp * cy;
	const double spcy = sp * cy;
	const double cpsy = cp * sy;
	const double spsy = sp * sy;

	i = (sr * cpcy - cr * spsy);
	j = (cr * spcy + sr * cpsy);
	k = (cr * cpsy - sr * spcy);
	w = (cr * cpcy + sr * spsy);

	Normalize();
}

DoubleQuaternion::DoubleQuaternion(DoubleMat44 const& mat)
{
	const double diag = mat.m_values[mat.Ix] + mat.m_values[mat.Jy] + mat.m_values[mat.Kz] + 1;

	if (diag > 0.0f)
	{
		const double scale = sqrt(diag) * 2.0f; // get scale from diagonal

		i = (mat.m_values[mat.Jz] - mat.m_values[mat.Ky]) / scale;
		j = (mat.m_values[mat.Kx] - mat.m_values[mat.Iz]) / scale;
		k = (mat.m_values[mat.Iy] - mat.m_values[mat.Jx]) / scale;
		w = 0.25f * scale;
	}
	else
	{
		if (mat.m_values[mat.Ix] > mat.m_values[mat.Jy] && mat.m_values[mat.Ix] > mat.m_values[mat.Kz])
		{
			// 1st element of diag is greatest value
			// find scale according to 1st element, and double it
			const double scale = sqrt(1.0f + mat.m_values[mat.Ix] - mat.m_values[mat.Jy] - mat.m_values[mat.Kz]) * 2.0f;
			i = 0.25f * scale;
			j = (mat.m_values[mat.Jx] + mat.m_values[mat.Iy]) / scale;
			k = (mat.m_values[mat.Iz] + mat.m_values[mat.Kx]) / scale;
			w = (mat.m_values[mat.Jz] - mat.m_values[mat.Ky]) / scale;
		}
		else if (mat.m_values[mat.Jy] > mat.m_values[mat.Kz])
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			const double scale = sqrt(1.0f + mat.m_values[mat.Jy] - mat.m_values[mat.Ix] - mat.m_values[mat.Kz]) * 2.0f;
			i = (mat.m_values[mat.Jx] + mat.m_values[mat.Iy]) / scale;
			j = 0.25f * scale;
			k = (mat.m_values[mat.Ky] + mat.m_values[mat.Jz]) / scale;
			w = (mat.m_values[mat.Kx] - mat.m_values[mat.Iz]) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			const double scale = sqrt(1.0f + mat.m_values[mat.Kz] - mat.m_values[mat.Ix] - mat.m_values[mat.Jy]) * 2.0f;
			i = (mat.m_values[mat.Kx] + mat.m_values[mat.Iz]) / scale;
			j = (mat.m_values[mat.Ky] + mat.m_values[mat.Jz]) / scale;
			k = 0.25f * scale;
			w = (mat.m_values[mat.Iy] - mat.m_values[mat.Jx]) / scale;
		}

	}
	Normalize();
}

DoubleQuaternion::DoubleQuaternion(DoubleQuaternion const& copyFrom)
	:i(copyFrom.i), j(copyFrom.j), k(copyFrom.k), w(copyFrom.w)
{

}

DoubleQuaternion::DoubleQuaternion(DoubleVec3 const& rotationVec)
	:i(rotationVec.x), j(rotationVec.y), k(rotationVec.z), w(0)
{

}

DoubleQuaternion::DoubleQuaternion(double angle, DoubleVec3 const& axis)
{
	double halfAngle = angle / 2;
	double cosAngle = cos(halfAngle);
	double sinAngle = sin(halfAngle);

	w = cosAngle;
	i = sinAngle * axis.x;
	j = sinAngle * axis.y;
	k = sinAngle * axis.z;
}

DoubleQuaternion::DoubleQuaternion(Quaternion q)
{
	i = static_cast<double>(q.i);
	j = static_cast<double>(q.j);
	k = static_cast<double>(q.k);
	w = static_cast<double>(q.w);
}

DoubleMat44 DoubleQuaternion::GetMatrix(DoubleVec3 translation) const
{
	DoubleMat44 result;

	result.m_values[result.Ix] = 1.0f - 2.0f * j * j - 2.0f * k * k;
	result.m_values[result.Iy] = 2.0f * i * j + 2.0f * k * w;
	result.m_values[result.Iz] = 2.0f * i * k - 2.0f * j * w;
	result.m_values[result.Iw] = 0.0f;

	result.m_values[result.Jx] = 2.0f * i * j - 2.0f * k * w;
	result.m_values[result.Jy] = 1.0f - 2.0f * i * i - 2.0f * k * k;
	result.m_values[result.Jz] = 2.0f * k * j + 2.0f * i * w;
	result.m_values[result.Jw] = 0.0f;

	result.m_values[result.Kx] = 2.0f * i * k + 2.0f * j * w;
	result.m_values[result.Ky] = 2.0f * k * j - 2.0f * i * w;
	result.m_values[result.Kz] = 1.0f - 2.0f * i * i - 2.0f * j * j;
	result.m_values[result.Kw] = 0.0f;

	result.m_values[result.Tx] = translation.x;
	result.m_values[result.Ty] = translation.y;
	result.m_values[result.Tz] = translation.z;
	result.m_values[result.Tw] = 1.f;

	return result;
}

//DoubleDoubleMat44 DoubleQuaternion::GetMatrix(DoubleDoubleVec3 translation /*= DoubleDoubleVec3()*/) const
//{
//	DoubleDoubleMat44 result;
//
//	result.m_values[result.Ix] = 1.0 - 2.0 * j * j - 2.0 * k * k;
//	result.m_values[result.Iy] = 2.0 * i * j + 2.0 * k * w;
//	result.m_values[result.Iz] = 2.0 * i * k - 2.0 * j * w;
//	result.m_values[result.Iw] = 0.0;
//
//	result.m_values[result.Jx] = 2.0 * i * j - 2.0 * k * w;
//	result.m_values[result.Jy] = 1.0 - 2.0 * i * i - 2.0 * k * k;
//	result.m_values[result.Jz] = 2.0 * k * j + 2.0 * i * w;
//	result.m_values[result.Jw] = 0.0;
//
//	result.m_values[result.Kx] = 2.0 * i * k + 2.0 * j * w;
//	result.m_values[result.Ky] = 2.0 * k * j - 2.0 * i * w;
//	result.m_values[result.Kz] = 1.0 - 2.0f * i * i - 2.0 * j * j;
//	result.m_values[result.Kw] = 0.0;
//
//	result.m_values[result.Tx] = translation.x;
//	result.m_values[result.Ty] = translation.y;
//	result.m_values[result.Tz] = translation.z;
//	result.m_values[result.Tw] = 1;
//
//	return result;
//}

DoubleQuaternion DoubleQuaternion::GetNormalized() const
{
	const double n = i * i + j * j + k * k + w * w;

	if (n == 1.f)
	{
		return *this;
	}

	double oneOverSqrt = 1.f / sqrt(n);
	return { i * oneOverSqrt, j * oneOverSqrt, k * oneOverSqrt, w * oneOverSqrt };
}

void DoubleQuaternion::Normalize()
{
	const double n = i * i + j * j + k * k + w * w;

	if (n != 1.f)
	{
		double oneOverSqrt = 1.f / sqrt(n);
		*this = (DoubleQuaternion(i * oneOverSqrt, j * oneOverSqrt, k * oneOverSqrt, w * oneOverSqrt));
	}
}

EulerAngles DoubleQuaternion::ToEuler()
{
	EulerAngles euler;

	const double wSq = w * w;
	const double iSq = i * i;
	const double jSq = j * j;
	const double kSq = k * k;
	const double test = 2.0f * (j * w - i * k);

	if (DoubleEqual(test, 1.0f, 0.000001f))
	{
		// heading = rotation about z-axis
		euler.m_yawDegrees = (float)(-2.0 * atan2(i, w));
		// bank = rotation about x-axis
		euler.m_rollDegrees = 0;
		// attitude = rotation about y-axis
		euler.m_pitchDegrees = (float)(PI / 2.0);
	}
	else if (DoubleEqual(test, -1.0f, 0.000001f))
	{
		// heading = rotation about z-axis
		euler.m_yawDegrees = (float)(2.0 * atan2(i, w));
		// bank = rotation about x-axis
		euler.m_rollDegrees = 0;
		// attitude = rotation about y-axis
		euler.m_pitchDegrees = (float)(PI / -2.0);
	}
	else
	{
		// heading = rotation about z-axis
		euler.m_yawDegrees = (float)atan2(2.0 * (i * j + k * w), (iSq - jSq - kSq + wSq));
		// bank = rotation about x-axis
		euler.m_rollDegrees = (float)atan2(2.0 * (j * k + i * w), (-iSq - jSq + kSq + wSq));
		// attitude = rotation about y-axis
		euler.m_pitchDegrees = (float)asin(Clamp_Double(test, -1.0, 1.0));
	}

	return euler;
}

void DoubleQuaternion::ToAngleAxis(double& angleRadian, DoubleVec3& axis) const
{
	const double scale = sqrt(i * i + j * j + k * k);

	if (DoubleEqual(scale, 0.f, 0.0000001f) || w > 1.0f || w < -1.0f)
	{
		angleRadian = 0.0f;
		axis.x = 0.0f;
		axis.y = 1.0f;
		axis.z = 0.0f;
	}
	else
	{
		const double invscale = 1.f / scale;
		angleRadian = 2.0f * acos(w);
		axis.x = i * invscale;
		axis.y = j * invscale;
		axis.z = k * invscale;
	}
}

DoubleQuaternion DoubleQuaternion::RotationFromTo(const DoubleVec3& from, const DoubleVec3& to)
{
	//Based on Stan Melax's article in Game Programming Gems
	//Copy, since cannot modify local
	DoubleVec3 v0 = from;
	DoubleVec3 v1 = to;
	v0.Normalize();
	v1.Normalize();

	const double d = DotProduct3D_Double(v0, v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return {};
	}
	else if (d <= -1.0f) // exactly opposite
	{
		DoubleVec3 axis(1.0f, 0.f, 0.f);
		axis = CrossProduct3D_Double(axis, v0);
		if (axis.GetLengthSquared() == 0)
		{
			axis = DoubleVec3(0.f, 1.f, 0.f);
			axis = CrossProduct3D_Double(axis, v0);
		}
		// same as fromAngleAxis(core::PI, axis).normalize();
		return DoubleQuaternion(axis.x, axis.y, axis.z, 0).GetNormalized();
	}

	const double s = sqrt((1 + d) * 2); // optimize inv_sqrt
	const double invs = 1.f / s;
	const DoubleVec3 c = CrossProduct3D_Double(v0, v1) * invs;
	return DoubleQuaternion(c.x, c.y, c.z, s * 0.5f).GetNormalized();
}

DoubleQuaternion DoubleQuaternion::Lerp(DoubleQuaternion q1, DoubleQuaternion q2, double zeroToOne)
{
	double oneMinus = 1.0f - zeroToOne;
	return (q1 * oneMinus) + (q2 * zeroToOne);
}

DoubleQuaternion DoubleQuaternion::SLerp(DoubleQuaternion q1, DoubleQuaternion q2, double t)
{
	DoubleQuaternion result;

	double dot = q1.DotProduct(q2);
	if (abs(dot) >= 1.0)
	{
		return q1;
	}
	double angle = acos(dot);
	double sinAngle = sin(angle);

	double alpha = sin((1 - t) * angle) / sinAngle;
	double beta = sin(t * angle) / sinAngle;

	result.w = alpha * q1.w + beta * q2.w;
	result.i = alpha * q1.i + beta * q2.i;
	result.j = alpha * q1.j + beta * q2.j;
	result.k = alpha * q1.k + beta * q2.k;

	return result;
}

DoubleQuaternion DoubleQuaternion::ComputeQuaternion(DoubleVec3 const& v)
{
	// Convert to axis-angle representation
	double angle = v.GetLength();
	if (angle < 1e-6f)
	{
		return Quaternion(0, 0, 0, 1); // Identity quaternion for tiny corrections
	}

	// Create quaternion from axis-angle
	double halfAngle = angle * 0.5f;
	double sinHalfAngle = sin(halfAngle);

	DoubleQuaternion result = DoubleQuaternion(v.x * sinHalfAngle, v.y * sinHalfAngle, v.z * sinHalfAngle, angle * cos(halfAngle)) * 1 / angle;

	return result;
}

DoubleVec3 DoubleQuaternion::ComputeAngleAxis(DoubleQuaternion const& q)
{
	double angle = 2 * acos(q.w);
	double term = angle / sin(angle / 2);

	DoubleVec3 v;
	v.x = term * q.i;
	v.y = term * q.j;
	v.z = term * q.k;

	return v;
}

bool DoubleQuaternion::Equal(DoubleQuaternion const& q, double tolerance)
{
	bool iEqual = i < q.i + tolerance && i > q.i - tolerance;
	bool jEqual = j < q.j + tolerance && j > q.j - tolerance;
	bool kEqual = k < q.k + tolerance && k > q.k - tolerance;
	bool wEqual = w < q.w + tolerance && w > q.w - tolerance;

	return iEqual && jEqual && kEqual && wEqual;
}

double DoubleQuaternion::DotProduct(DoubleQuaternion const& q) const
{
	return (i * q.i) + (j * q.j) + (k * q.k) + (w * q.w);
}

double DoubleQuaternion::GetAngle()
{
	return 2 * acos(w);
}

DoubleVec3 DoubleQuaternion::Rotate(const DoubleVec3& v) const
{
	DoubleQuaternion p(v);
	DoubleQuaternion qInv = this->GetConjugated();
	DoubleQuaternion rotated = *this * p * qInv;
	return { rotated.i, rotated.j, rotated.k };
}

double DoubleQuaternion::GetMagnitude() const
{
	return sqrt(w * w + i * i + j * j + k * k);
}

DoubleQuaternion DoubleQuaternion::GetConjugated() const
{
	return { -i, -j, -k, w };
}

void DoubleQuaternion::Conjugate()
{
	i = -i;
	j = -j;
	k = -k;
}

void DoubleQuaternion::Inverse()
{
	double norm = w * w + i * i + j * j + k * k;
	w /= norm;
	i /= -norm;
	j /= -norm;
	k /= -norm;
}

DoubleQuaternion DoubleQuaternion::GetInversed() const
{
	double norm = w * w + i * i + j * j + k * k;
	return { -i / norm, -j / norm, -k / norm, w / norm };
}

const DoubleQuaternion DoubleQuaternion::operator+(const DoubleQuaternion& qToAdd) const
{
	return DoubleQuaternion(i + qToAdd.i, j + qToAdd.j, k + qToAdd.k, w + qToAdd.w);
}

const DoubleQuaternion DoubleQuaternion::operator-(const DoubleQuaternion& qToSubtract) const
{
	return DoubleQuaternion(i - qToSubtract.i, j - qToSubtract.j, k - qToSubtract.k, w - qToSubtract.w);
}

const DoubleQuaternion DoubleQuaternion::operator*(double uniformScale) const
{
	return DoubleQuaternion(i * uniformScale, j * uniformScale, k * uniformScale, w * uniformScale);
}

const DoubleQuaternion DoubleQuaternion::operator*(const DoubleQuaternion& qToMultiply) const
{
	DoubleQuaternion tmp;

	tmp.w = (qToMultiply.w * w) - (qToMultiply.i * i) - (qToMultiply.j * j) - (qToMultiply.k * k);
	tmp.i = (qToMultiply.w * i) + (qToMultiply.i * w) + (qToMultiply.j * k) - (qToMultiply.k * j);
	tmp.j = (qToMultiply.w * j) + (qToMultiply.j * w) + (qToMultiply.k * i) - (qToMultiply.i * k);
	tmp.k = (qToMultiply.w * k) + (qToMultiply.k * w) + (qToMultiply.i * j) - (qToMultiply.j * i);

	return tmp;
}

void DoubleQuaternion::operator*=(const double uniformScale)
{
	i *= uniformScale;
	j *= uniformScale;
	k *= uniformScale;
	w *= uniformScale;
}

void DoubleQuaternion::operator*=(const DoubleQuaternion& qToMultiply)
{
	*this = (*this) * qToMultiply;
}

const DoubleVec3 DoubleQuaternion::operator*(DoubleVec3 vecToMultiply) const
{
	// nVidia SDK implementation

	DoubleVec3 uv, uuv;
	DoubleVec3 qvec(i, j, k);
	uv = CrossProduct3D_Double(qvec, vecToMultiply);
	uuv = CrossProduct3D_Double(qvec, uv);
	uv *= (2.0f * w);
	uuv *= 2.0f;

	return vecToMultiply + uv + uuv;
}

void DoubleQuaternion::operator=(const DoubleQuaternion& copjFrom)
{
	i = copjFrom.i;
	j = copjFrom.j;
	k = copjFrom.k;
	w = copjFrom.w;
}

void DoubleQuaternion::operator/=(const double uniformDivisor)
{
	double oneOverDivisor = 1.f / uniformDivisor;
	i *= oneOverDivisor;
	j *= oneOverDivisor;
	k *= oneOverDivisor;
	w *= oneOverDivisor;
}

void DoubleQuaternion::operator-=(const DoubleQuaternion& qToSubtract)
{
	i -= qToSubtract.i;
	j -= qToSubtract.j;
	k -= qToSubtract.k;
	w -= qToSubtract.w;
}

void DoubleQuaternion::operator+=(const DoubleQuaternion& qToAdd)
{
	i += qToAdd.i;
	j += qToAdd.j;
	k += qToAdd.k;
	w += qToAdd.w;
}

const DoubleQuaternion DoubleQuaternion::operator/(double inverseScale) const
{
	double oneOverScale = 1.f / inverseScale;
	return DoubleQuaternion(i * oneOverScale, j * oneOverScale, k * oneOverScale, w * oneOverScale);
}

bool DoubleQuaternion::operator!=(const DoubleQuaternion& compare) const
{
	return !(*this == compare);
}

bool DoubleQuaternion::operator==(const DoubleQuaternion& compare) const
{
	return i == compare.i && j == compare.j && k == compare.k && w == compare.w;
}
