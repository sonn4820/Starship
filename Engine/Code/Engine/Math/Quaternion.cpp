#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/MathUtils.hpp"

Quaternion::Quaternion(float i, float j, float k)
	:i(i), j(j), k(k)
{

}

Quaternion::Quaternion(float i, float j, float k, float w)
	:i(i), j(j), k(k), w(w)
{

}

Quaternion::Quaternion(EulerAngles const& eulerAngle)
{
	float angle;

	angle = eulerAngle.m_rollDegrees * 0.5f;
	const float sr = SinDegrees(angle);
	const float cr = CosDegrees(angle);

	angle = eulerAngle.m_pitchDegrees * 0.5f;
	const float sp = SinDegrees(angle);
	const float cp = CosDegrees(angle);

	angle = eulerAngle.m_yawDegrees * 0.5f;
	const float sy = SinDegrees(angle);
	const float cy = CosDegrees(angle);

	const float cpcy = cp * cy;
	const float spcy = sp * cy;
	const float cpsy = cp * sy;
	const float spsy = sp * sy;

	i = (sr * cpcy - cr * spsy);
	j = (cr * spcy + sr * cpsy);
	k = (cr * cpsy - sr * spcy);
	w = (cr * cpcy + sr * spsy);

	Normalize();
}

Quaternion::Quaternion(Mat44 const& mat)
{
	const float diag = mat.m_values[mat.Ix] + mat.m_values[mat.Jy] + mat.m_values[mat.Kz] + 1;

	if (diag > 0.0f)
	{
		const float scale = sqrtf(diag) * 2.0f; // get scale from diagonal

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
			const float scale = sqrtf(1.0f + mat.m_values[mat.Ix] - mat.m_values[mat.Jy] - mat.m_values[mat.Kz]) * 2.0f;
			i = 0.25f * scale;
			j = (mat.m_values[mat.Jx] + mat.m_values[mat.Iy]) / scale;
			k = (mat.m_values[mat.Iz] + mat.m_values[mat.Kx]) / scale;
			w = (mat.m_values[mat.Jz] - mat.m_values[mat.Ky]) / scale;
		}
		else if (mat.m_values[mat.Jy] > mat.m_values[mat.Kz])
		{
			// 2nd element of diag is greatest value
			// find scale according to 2nd element, and double it
			const float scale = sqrtf(1.0f + mat.m_values[mat.Jy] - mat.m_values[mat.Ix] - mat.m_values[mat.Kz]) * 2.0f;
			i = (mat.m_values[mat.Jx] + mat.m_values[mat.Iy]) / scale;
			j = 0.25f * scale;
			k = (mat.m_values[mat.Ky] + mat.m_values[mat.Jz]) / scale;
			w = (mat.m_values[mat.Kx] - mat.m_values[mat.Iz]) / scale;
		}
		else
		{
			// 3rd element of diag is greatest value
			// find scale according to 3rd element, and double it
			const float scale = sqrtf(1.0f + mat.m_values[mat.Kz] - mat.m_values[mat.Ix] - mat.m_values[mat.Jy]) * 2.0f;
			i = (mat.m_values[mat.Kx] + mat.m_values[mat.Iz]) / scale;
			j = (mat.m_values[mat.Ky] + mat.m_values[mat.Jz]) / scale;
			k = 0.25f * scale;
			w = (mat.m_values[mat.Iy] - mat.m_values[mat.Jx]) / scale;
		}

	}
	Normalize();
}

Quaternion::Quaternion(Quaternion const& copyFrom)
	:i(copyFrom.i), j(copyFrom.j), k(copyFrom.k), w(copyFrom.w)
{

}

Quaternion::Quaternion(Vec3 const& rotationVec)
	:i(rotationVec.x), j(rotationVec.y), k(rotationVec.z), w(0)
{

}

Quaternion::Quaternion(float angle, Vec3 const& axis)
{
	float halfAngle = angle / 2;
	float cosAngle = cosf(halfAngle);
	float sinAngle = sinf(halfAngle);

	w = cosAngle;
	i = sinAngle * axis.x;
	j = sinAngle * axis.y;
	k = sinAngle * axis.z;
}

Quaternion::Quaternion(DoubleQuaternion q)
{
	i = static_cast<float>(q.i);
	j = static_cast<float>(q.j);
	k = static_cast<float>(q.k);
	w = static_cast<float>(q.w);
}

Mat44 Quaternion::GetMatrix(Vec3 translation) const
{
	Mat44 result;

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

//DoubleMat44 Quaternion::GetMatrix(DoubleVec3 translation /*= DoubleVec3()*/) const
//{
//	DoubleMat44 result;
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

Quaternion Quaternion::GetNormalized() const
{
	const float n = i * i + j * j + k * k + w * w;

	if (n == 1.f)
	{
		return *this;
	}

	float oneOverSqrt = 1.f / sqrtf(n);
	return { i * oneOverSqrt, j * oneOverSqrt, k * oneOverSqrt, w * oneOverSqrt };
}

void Quaternion::Normalize()
{
	const float n = i * i + j * j + k * k + w * w;

	if (n != 1.f)
	{
		float oneOverSqrt = 1.f / sqrtf(n);
		*this = (Quaternion(i * oneOverSqrt, j * oneOverSqrt, k * oneOverSqrt, w * oneOverSqrt));
	}
}

EulerAngles Quaternion::ToEuler()
{
	EulerAngles euler;

	const float wSq = w * w;
	const float iSq = i * i;
	const float jSq = j * j;
	const float kSq = k * k;
	const float test = 2.0f * (j * w - i * k);

	if (FloatEqual(test, 1.0f, 0.000001f))
	{
		// heading = rotation about z-axis
		euler.m_yawDegrees = (float)(-2.0 * atan2(i, w));
		// bank = rotation about x-axis
		euler.m_rollDegrees = 0;
		// attitude = rotation about y-axis
		euler.m_pitchDegrees = (float)(PI / 2.0);
	}
	else if (FloatEqual(test, -1.0f, 0.000001f))
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
		euler.m_pitchDegrees = (float)asin(Clamp(test, -1.0, 1.0));
	}

	return euler;
}

void Quaternion::ToAngleAxis(float& angleRadian, Vec3& axis) const
{
	const float scale = sqrtf(i * i + j * j + k * k);

	if (FloatEqual(scale, 0.f, 0.0000001f) || w > 1.0f || w < -1.0f)
	{
		angleRadian = 0.0f;
		axis.x = 0.0f;
		axis.y = 1.0f;
		axis.z = 0.0f;
	}
	else
	{
		const float invscale = 1.f / scale;
		angleRadian = 2.0f * acosf(w);
		axis.x = i * invscale;
		axis.y = j * invscale;
		axis.z = k * invscale;
	}
}

Quaternion Quaternion::RotationFromTo(const Vec3& from, const Vec3& to)
{
	//Based on Stan Melax's article in Game Programming Gems
	//Copy, since cannot modify local
	Vec3 v0 = from;
	Vec3 v1 = to;
	v0.Normalize();
	v1.Normalize();

	const float d = DotProduct3D(v0, v1);
	if (d >= 1.0f) // If dot == 1, vectors are the same
	{
		return {};
	}
	else if (d <= -1.0f) // exactly opposite
	{
		Vec3 axis(1.0f, 0.f, 0.f);
		axis = CrossProduct3D(axis, v0);
		if (axis.GetLengthSquared() == 0)
		{
			axis = Vec3(0.f, 1.f, 0.f);
			axis = CrossProduct3D(axis, v0);
		}
		// same as fromAngleAxis(core::PI, axis).normalize();
		return Quaternion(axis.x, axis.y, axis.z, 0).GetNormalized();
	}

	const float s = sqrtf((1 + d) * 2); // optimize inv_sqrt
	const float invs = 1.f / s;
	const Vec3 c = CrossProduct3D(v0, v1) * invs;
	return Quaternion(c.x, c.y, c.z, s * 0.5f).GetNormalized();
}

Quaternion Quaternion::Lerp(Quaternion q1, Quaternion q2, float zeroToOne)
{
	float oneMinus = 1.0f - zeroToOne;
	return (q1 * oneMinus) + (q2 * zeroToOne);
}

Quaternion Quaternion::SLerp(Quaternion q1, Quaternion q2, float deltaTime, float threshold)
{
	float angle = q1.DotProduct(q2);

	// make sure we use the short rotation
	if (angle < 0.0f)
	{
		q1 *= -1.0f;
		angle *= -1.0f;
	}

	if (angle <= (1 - threshold)) // spherical interpolation
	{
		const float theta = acosf(angle);
		const float invsintheta = 1 / (sinf(theta));
		const float scale = sinf(theta * (1.0f - deltaTime)) * invsintheta;
		const float invscale = sinf(theta * deltaTime) * invsintheta;
		return (q1 * scale) + (q2 * invscale);
	}
	else
	{
		return Lerp(q1, q2, deltaTime);
	}
}

bool Quaternion::Equal(Quaternion const& q, float tolerance)
{
	bool iEqual = i < q.i + tolerance && i > q.i - tolerance;
	bool jEqual = j < q.j + tolerance && j > q.j - tolerance;
	bool kEqual = k < q.k + tolerance && k > q.k - tolerance;
	bool wEqual = w < q.w + tolerance && w > q.w - tolerance;

	return iEqual && jEqual && kEqual && wEqual;
}

float Quaternion::DotProduct(Quaternion& q)
{
	return (i * q.i) + (j * q.j) + (k * q.k) + (w * q.w);
}

float Quaternion::GetAngle()
{
	return 2 * acosf(w);
}

Vec3 Quaternion::Rotate(const Vec3& v)
{
	Quaternion p(v);
	Quaternion qInv = this->GetConjugated();
	Quaternion rotated = *this * p * qInv;
	return { rotated.i, rotated.j, rotated.k };
}

float Quaternion::GetMagnitude() const
{
	return sqrtf(w * w + i * i + j * j + k * k);
}

Quaternion Quaternion::GetConjugated() const
{
	return { -i, -j, -k, w };
}

void Quaternion::Conjugate()
{
	i = -i;
	j = -j;
	k = -k;
}

void Quaternion::Inverse()
{
	float norm = w * w + i * i + j * j + k * k;
	w /= norm;
	i /= -norm;
	j /= -norm;
	k /= -norm;
}

Quaternion Quaternion::GetInversed() const
{
	float norm = w * w + i * i + j * j + k * k;
	return { -i / norm, -j / norm, -k / norm, w / norm };
}

const Quaternion Quaternion::operator+(const Quaternion& qToAdd) const
{
	return Quaternion(i + qToAdd.i, j + qToAdd.j, k + qToAdd.k, w + qToAdd.w);
}

const Quaternion Quaternion::operator-(const Quaternion& qToSubtract) const
{
	return Quaternion(i - qToSubtract.i, j - qToSubtract.j, k - qToSubtract.k, w - qToSubtract.w);
}

const Quaternion Quaternion::operator*(float uniformScale) const
{
	return Quaternion(i * uniformScale, j * uniformScale, k * uniformScale, w * uniformScale);
}

const Quaternion Quaternion::operator*(const Quaternion& qToMultiply) const
{
	Quaternion tmp;

	tmp.w = (qToMultiply.w * w) - (qToMultiply.i * i) - (qToMultiply.j * j) - (qToMultiply.k * k);
	tmp.i = (qToMultiply.w * i) + (qToMultiply.i * w) + (qToMultiply.j * k) - (qToMultiply.k * j);
	tmp.j = (qToMultiply.w * j) + (qToMultiply.j * w) + (qToMultiply.k * i) - (qToMultiply.i * k);
	tmp.k = (qToMultiply.w * k) + (qToMultiply.k * w) + (qToMultiply.i * j) - (qToMultiply.j * i);

	return tmp;
}

void Quaternion::operator*=(const float uniformScale)
{
	i *= uniformScale;
	j *= uniformScale;
	k *= uniformScale;
	w *= uniformScale;
}

void Quaternion::operator*=(const Quaternion& qToMultiply)
{
	*this = (*this) * qToMultiply;
}

const Vec3 Quaternion::operator*(Vec3 vecToMultiply) const
{
	// nVidia SDK implementation

	Vec3 uv, uuv;
	Vec3 qvec(i, j, k);
	uv = CrossProduct3D(qvec, vecToMultiply);
	uuv = CrossProduct3D(qvec, uv);
	uv *= (2.0f * w);
	uuv *= 2.0f;

	return vecToMultiply + uv + uuv;
}

void Quaternion::operator=(const Quaternion& copjFrom)
{
	i = copjFrom.i;
	j = copjFrom.j;
	k = copjFrom.k;
	w = copjFrom.w;
}

void Quaternion::operator/=(const float uniformDivisor)
{
	float oneOverDivisor = 1.f / uniformDivisor;
	i *= oneOverDivisor;
	j *= oneOverDivisor;
	k *= oneOverDivisor;
	w *= oneOverDivisor;
}

void Quaternion::operator-=(const Quaternion& qToSubtract)
{
	i -= qToSubtract.i;
	j -= qToSubtract.j;
	k -= qToSubtract.k;
	w -= qToSubtract.w;
}

void Quaternion::operator+=(const Quaternion& qToAdd)
{
	i += qToAdd.i;
	j += qToAdd.j;
	k += qToAdd.k;
	w += qToAdd.w;
}

const Quaternion Quaternion::operator/(float inverseScale) const
{
	float oneOverScale = 1.f / inverseScale;
	return Quaternion(i * oneOverScale, j * oneOverScale, k * oneOverScale, w * oneOverScale);
}

bool Quaternion::operator!=(const Quaternion& compare) const
{
	return !(*this == compare);
}

bool Quaternion::operator==(const Quaternion& compare) const
{
	return i == compare.i && j == compare.j && k == compare.k && w == compare.w;
}
