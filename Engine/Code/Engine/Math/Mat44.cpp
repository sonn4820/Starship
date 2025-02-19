#include "Engine/Math/Mat44.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/Quaternion.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

Mat44::Mat44()
{
	m_values[Ix] = 1;
	m_values[Jx] = 0;
	m_values[Kx] = 0;
	m_values[Tx] = 0;

	m_values[Iy] = 0;
	m_values[Jy] = 1;
	m_values[Ky] = 0;
	m_values[Ty] = 0;

	m_values[Iz] = 0;
	m_values[Jz] = 0;
	m_values[Kz] = 1;
	m_values[Tz] = 0;

	m_values[Iw] = 0;
	m_values[Jw] = 0;
	m_values[Kw] = 0;
	m_values[Tw] = 1;

}

Mat44::Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Jx] = jBasis2D.x;
	m_values[Kx] = 0;
	m_values[Tx] = translation2D.x;

	m_values[Iy] = iBasis2D.y;
	m_values[Jy] = jBasis2D.y;
	m_values[Ky] = 0;
	m_values[Ty] = translation2D.y;

	m_values[Iz] = 0;
	m_values[Jz] = 0;
	m_values[Kz] = 1;
	m_values[Tz] = 0;

	m_values[Iw] = 0;
	m_values[Jw] = 0;
	m_values[Kw] = 0;
	m_values[Tw] = 1;
}

Mat44::Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Jx] = jBasis3D.x;
	m_values[Kx] = kBasis3D.x;
	m_values[Tx] = translation3D.x;

	m_values[Iy] = iBasis3D.y;
	m_values[Jy] = jBasis3D.y;
	m_values[Ky] = kBasis3D.y;
	m_values[Ty] = translation3D.y;

	m_values[Iz] = iBasis3D.z;
	m_values[Jz] = jBasis3D.z;
	m_values[Kz] = kBasis3D.z;
	m_values[Tz] = translation3D.z;

	m_values[Iw] = 0;
	m_values[Jw] = 0;
	m_values[Kw] = 0;
	m_values[Tw] = 1;
}

Mat44::Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Jx] = jBasis4D.x;
	m_values[Kx] = kBasis4D.x;
	m_values[Tx] = translation4D.x;

	m_values[Iy] = iBasis4D.y;
	m_values[Jy] = jBasis4D.y;
	m_values[Ky] = kBasis4D.y;
	m_values[Ty] = translation4D.y;

	m_values[Iz] = iBasis4D.z;
	m_values[Jz] = jBasis4D.z;
	m_values[Kz] = kBasis4D.z;
	m_values[Tz] = translation4D.z;

	m_values[Iw] = iBasis4D.w;
	m_values[Jw] = jBasis4D.w;
	m_values[Kw] = kBasis4D.w;
	m_values[Tw] = translation4D.w;
}

Mat44::Mat44(float const* sixteenValuesBasisMajor)
{
	for (int i = 0; i < 16; i++)
	{
		m_values[i] = sixteenValuesBasisMajor[i];
	}
}

Mat44::Mat44(DoubleMat44 dmat)
{
	m_values[Ix] = static_cast<float>(dmat.m_values[Ix]);
	m_values[Jx] = static_cast<float>(dmat.m_values[Jx]);
	m_values[Kx] = static_cast<float>(dmat.m_values[Kx]);
	m_values[Tx] = static_cast<float>(dmat.m_values[Tx]);

	m_values[Iy] = static_cast<float>(dmat.m_values[Iy]);
	m_values[Jy] = static_cast<float>(dmat.m_values[Jy]);
	m_values[Ky] = static_cast<float>(dmat.m_values[Ky]);
	m_values[Ty] = static_cast<float>(dmat.m_values[Ty]);

	m_values[Iz] = static_cast<float>(dmat.m_values[Iz]);
	m_values[Jz] = static_cast<float>(dmat.m_values[Jz]);
	m_values[Kz] = static_cast<float>(dmat.m_values[Kz]);
	m_values[Tz] = static_cast<float>(dmat.m_values[Tz]);

	m_values[Iw] = static_cast<float>(dmat.m_values[Iw]);
	m_values[Jw] = static_cast<float>(dmat.m_values[Jw]);
	m_values[Kw] = static_cast<float>(dmat.m_values[Kw]);
	m_values[Tw] = static_cast<float>(dmat.m_values[Tw]);
}

Mat44 const Mat44::CreateTranslation2D(Vec2 const& translationXY)
{
	Mat44 mat = Mat44();
	mat.m_values[Tx] = translationXY.x;
	mat.m_values[Ty] = translationXY.y;
	return mat;
}

Mat44 const Mat44::CreateTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 mat = Mat44();
	mat.m_values[Tx] = translationXYZ.x;
	mat.m_values[Ty] = translationXYZ.y;
	mat.m_values[Tz] = translationXYZ.z;
	return mat;
}

Mat44 const Mat44::CreateUniformScale2D(float uniformScaleXY)
{
	Mat44 mat = Mat44();
	mat.m_values[Ix] = uniformScaleXY;
	mat.m_values[Jy] = uniformScaleXY;
	return mat;
}

Mat44 const Mat44::CreateUniformScale3D(float uniformScaleXYZ)
{
	Mat44 mat = Mat44();
	mat.m_values[Ix] = uniformScaleXYZ;
	mat.m_values[Jy] = uniformScaleXYZ;
	mat.m_values[Kz] = uniformScaleXYZ;
	return mat;
}

Mat44 const Mat44::CreateNonUniformScale2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 mat = Mat44();
	mat.m_values[Ix] = nonUniformScaleXY.x;
	mat.m_values[Jy] = nonUniformScaleXY.y;
	return mat;
}

Mat44 const Mat44::CreateNonUniformScale3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 mat = Mat44();
	mat.m_values[Ix] = nonUniformScaleXYZ.x;
	mat.m_values[Jy] = nonUniformScaleXYZ.y;
	mat.m_values[Kz] = nonUniformScaleXYZ.z;
	return mat;
}

Mat44 const Mat44::CreateZRotationDegrees(float rotationDegreesAboutZ)
{
	float cosAngle = CosDegrees(rotationDegreesAboutZ);
	float sinAngle = SinDegrees(rotationDegreesAboutZ);

	Mat44 mat = Mat44();
	mat.m_values[Ix] = cosAngle;
	mat.m_values[Iy] = sinAngle;
	mat.m_values[Jx] = -sinAngle;
	mat.m_values[Jy] = cosAngle;
	return mat;
}

Mat44 const Mat44::CreateYRotationDegrees(float rotationDegreesAboutY)
{
	float cosAngle = CosDegrees(rotationDegreesAboutY);
	float sinAngle = SinDegrees(rotationDegreesAboutY);

	Mat44 mat = Mat44();
	mat.m_values[Ix] = cosAngle;
	mat.m_values[Iz] = -sinAngle;
	mat.m_values[Kx] = sinAngle;
	mat.m_values[Kz] = cosAngle;
	return mat;
}

Mat44 const Mat44::CreateXRotationDegrees(float rotationDegreesAboutX)
{
	float cosAngle = CosDegrees(rotationDegreesAboutX);
	float sinAngle = SinDegrees(rotationDegreesAboutX);

	Mat44 mat = Mat44();
	mat.m_values[Jy] = cosAngle;
	mat.m_values[Jz] = sinAngle;
	mat.m_values[Ky] = -sinAngle;
	mat.m_values[Kz] = cosAngle;
	return mat;
}

Mat44 const Mat44::CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar)
{
	Mat44 ortho = Mat44();
	ortho.m_values[Ix] = 2.f / (right - left);
	ortho.m_values[Jy] = 2.f / (top - bottom);
	ortho.m_values[Kz] = 1.f / (zFar - zNear);
	ortho.m_values[Tx] = (left + right) / (left - right);
	ortho.m_values[Ty] = (bottom + top) / (bottom - top);
	ortho.m_values[Tz] = zNear / (zNear - zFar);
	return ortho;
}

Mat44 const Mat44::CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar)
{
	Mat44 perspective = Mat44();

	float radFov = ConvertDegreesToRadians(fovYDegrees);
	float focalLength = 1.f / tanf(radFov / 2.f);
	float zRange = zFar - zNear;

	perspective.m_values[Ix] = focalLength / aspect;
	perspective.m_values[Jy] = focalLength;
	perspective.m_values[Kz] = zFar / zRange;
	perspective.m_values[Kw] = 1.f;

	perspective.m_values[Tz] = -(zFar * zNear) / zRange;
	perspective.m_values[Tw] = 0.f;

	return perspective;
}

Mat44 const Mat44::CreateLookForward(Vec3 const& iBasis)
{
	Mat44 lookAt = Mat44();

	Vec3 kBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(iBasis, Vec3(0.f, 0.f, 1.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	lookAt.SetIJK3D(iBasis, jBasis, kBasis);

	return lookAt;
}

Mat44 const Mat44::TransformWorldToLocal(Mat44 const& worldMat)
{
	Mat44 localMat = worldMat;
	localMat.GetOrthonormalInverse();
	return localMat;
}

Vec2 const Mat44::TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const
{
	return (GetIBasis2D() * vectorQuantityXY.x) + (GetJBasis2D() * vectorQuantityXY.y);
}

Vec3 const Mat44::TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const
{
	return (GetIBasis3D() * vectorQuantityXYZ.x) + (GetJBasis3D() * vectorQuantityXYZ.y) + (GetKBasis3D() * vectorQuantityXYZ.z);
}

Vec2 const Mat44::TransformPosition2D(Vec2 const& positionXY) const
{
	return (GetIBasis2D() * positionXY.x) + (GetJBasis2D() * positionXY.y) + GetTranslation2D();
}

Vec3 const Mat44::TransformPosition3D(Vec3 const& positionXYZ) const
{
	return (GetIBasis3D() * positionXYZ.x) + (GetJBasis3D() * positionXYZ.y) + (GetKBasis3D() * positionXYZ.z) + GetTranslation3D();
}

Vec4 const Mat44::TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const
{
	return (GetIBasis4D() * homogeneousPoint3D.x) + (GetJBasis4D() * homogeneousPoint3D.y) + (GetKBasis4D() * homogeneousPoint3D.z) + (GetTranslation4D() * homogeneousPoint3D.w);
}

float* Mat44::GetAsFloatArray()
{
	return m_values;
}

float const* Mat44::GetAsFloatArray() const
{
	return m_values;
}

Vec2 Mat44::GetIBasis2D() const
{
	return Vec2(m_values[Ix], m_values[Iy]);
}

Vec2 Mat44::GetJBasis2D() const
{
	return Vec2(m_values[Jx], m_values[Jy]);
}

Vec2 Mat44::GetTranslation2D() const
{
	return Vec2(m_values[Tx], m_values[Ty]);
}

Vec3 Mat44::GetIBasis3D() const
{
	return Vec3(m_values[Ix], m_values[Iy], m_values[Iz]);
}

Vec3 Mat44::GetJBasis3D() const
{
	return Vec3(m_values[Jx], m_values[Jy], m_values[Jz]);
}

Vec3 Mat44::GetKBasis3D() const
{
	return Vec3(m_values[Kx], m_values[Ky], m_values[Kz]);
}

Vec3 Mat44::GetTranslation3D() const
{
	return Vec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}

Vec4 Mat44::GetIBasis4D() const
{
	return Vec4(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
}

Vec4 Mat44::GetJBasis4D() const
{
	return Vec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}

Vec4 Mat44::GetKBasis4D() const
{
	return Vec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}

Vec4 Mat44::GetTranslation4D() const
{
	return Vec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}

Mat44 Mat44::GetOrthonormalInverse() const
{
	Mat44 antiRotation;
	antiRotation.SetIJK3D(GetIBasis3D(), GetJBasis3D(), GetKBasis3D());
	antiRotation.Transpose();

	Mat44 antiTranslation;
	antiTranslation.SetTranslation3D(-1.f * GetTranslation3D());

	Mat44 result = antiRotation;
	result.Append(antiTranslation);

	return result;
}

Mat44 Mat44::GetLookAtTarget(Vec3 const& targetPosition) const
{
	Mat44 result;

	Vec3 axis = targetPosition - GetTranslation3D();
	Vec3 iBasis = axis.GetNormalized();
	Vec3 kBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(iBasis, Vec3(0.f, 0.f, 1.f))) < 0.9999f)
	{
		jBasis = CrossProduct3D(Vec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	result.SetIJK3D(iBasis, jBasis, kBasis);
	return result;
}

EulerAngles Mat44::GetEulerAngle() const
{
	EulerAngles result;
	if (m_values[Kz] < 1)
	{
		if (m_values[Kz] > -1)
		{
			result.m_pitchDegrees = asinf(m_values[Kz]);
			result.m_yawDegrees = atan2f(-m_values[Jz], m_values[Iz]);
			result.m_rollDegrees = atan2f(-m_values[Ky], m_values[Kx]);
		}
		else
		{
			result.m_pitchDegrees = -PI / 2;
			result.m_yawDegrees = -atan2f(m_values[Jy], m_values[Jx]);
			result.m_rollDegrees = 0.f;
		}
	}
	else
	{
		result.m_pitchDegrees = PI / 2;
		result.m_yawDegrees = atan2f(m_values[Jy], m_values[Jx]);
		result.m_rollDegrees = 0.f;
	}

	result.m_pitchDegrees = ConvertRadiansToDegrees(result.m_pitchDegrees);
	result.m_yawDegrees = ConvertRadiansToDegrees(result.m_yawDegrees);
	result.m_rollDegrees = ConvertRadiansToDegrees(result.m_rollDegrees);

	return result;
}

Quaternion Mat44::GetQuaternion() const
{
	return 	Quaternion(*this);
}

Mat44 Mat44::GetInverseRotationMatrix() const
{
	Mat44 result;
	// Copy the 3x3 rotation part in transposed order
	result.m_values[Ix] = m_values[Ix];
	result.m_values[Iy] = m_values[Jx];
	result.m_values[Iz] = m_values[Kx];

	result.m_values[Jx] = m_values[Iy];
	result.m_values[Jy] = m_values[Jy];
	result.m_values[Jz] = m_values[Ky];

	result.m_values[Kx] = m_values[Iz];
	result.m_values[Ky] = m_values[Jz];
	result.m_values[Kz] = m_values[Kz];

	// Set translation to zero
	result.m_values[Tx] = 0.0f;
	result.m_values[Ty] = 0.0f;
	result.m_values[Tz] = 0.0f;
	result.m_values[Tw] = 1.0f;

	return result;
}

void Mat44::LookAtTarget(Vec3 const& targetPosition)
{
	Vec3 axis = targetPosition - GetTranslation3D();
	Vec3 iBasis = axis.GetNormalized();
	Vec3 kBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(iBasis, Vec3(0.f, 0.f, 1.f))) < 0.9999f)
	{
		jBasis = CrossProduct3D(Vec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	SetIJK3D(iBasis, jBasis, kBasis);
}

void Mat44::LookAtTargetXY(Vec3 const& targetPosition)
{
	Vec3 axis = targetPosition - GetTranslation3D();
	Vec3 iBasis = axis.GetNormalized();
	Vec3 kBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(iBasis, Vec3(0.f, 0.f, 1.f))) != 1.f)
	{
		jBasis = CrossProduct3D(Vec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	SetIJK3D(iBasis, jBasis, Vec3(0.f, 0.f, 1.f));
}

void Mat44::SetTranslation2D(Vec2 const& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0;
	m_values[Tw] = 1;
}

void Mat44::SetTranslation3D(Vec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1;
}

void Mat44::SetQuaternion(Quaternion q)
{
	*this = q.GetMatrix(this->GetTranslation3D());
}

void Mat44::SetEulerAngle(EulerAngles a)
{
	float Cy = CosDegrees(a.m_yawDegrees);
	float Sy = SinDegrees(a.m_yawDegrees);
	float Cp = CosDegrees(a.m_pitchDegrees);
	float Sp = SinDegrees(a.m_pitchDegrees);
	float Cr = CosDegrees(a.m_rollDegrees);
	float Sr = SinDegrees(a.m_rollDegrees);

	Vec3 forwardIBasis = Vec3(Cy * Cp, Cp * Sy, -Sp);
	Vec3 leftJBasis = Vec3(Sr * Sp * Cy - Sy * Cr, Sr * Sp * Sy + Cr * Cy, Sr * Cp);
	Vec3 upKBasis = Vec3(Sp * Cr * Cy + Sy * Sr, Sp * Cr * Sy - Sr * Cy, Cp * Cr);

	SetIJK3D(forwardIBasis, leftJBasis, upKBasis);
}

void Mat44::SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D)
{
	m_values[Ix] = iBasis2D.x;
	m_values[Iy] = iBasis2D.y;
	m_values[Iz] = 0;
	m_values[Iw] = 0;

	m_values[Jx] = jBasis2D.x;
	m_values[Jy] = jBasis2D.y;
	m_values[Jz] = 0;
	m_values[Jw] = 0;
}

void Mat44::SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY)
{
	SetIJ2D(iBasis2D, jBasis2D);
	SetTranslation2D(translationXY);
}

void Mat44::SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D)
{
	m_values[Ix] = iBasis3D.x;
	m_values[Iy] = iBasis3D.y;
	m_values[Iz] = iBasis3D.z;
	m_values[Iw] = 0;

	m_values[Jx] = jBasis3D.x;
	m_values[Jy] = jBasis3D.y;
	m_values[Jz] = jBasis3D.z;
	m_values[Jw] = 0;

	m_values[Kx] = kBasis3D.x;
	m_values[Ky] = kBasis3D.y;
	m_values[Kz] = kBasis3D.z;
	m_values[Kw] = 0;
}

void Mat44::SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ)
{
	SetIJK3D(iBasis3D, jBasis3D, kBasis3D);
	SetTranslation3D(translationXYZ);
}

void Mat44::SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D)
{
	m_values[Ix] = iBasis4D.x;
	m_values[Jx] = jBasis4D.x;
	m_values[Kx] = kBasis4D.x;
	m_values[Tx] = translation4D.x;

	m_values[Iy] = iBasis4D.y;
	m_values[Jy] = jBasis4D.y;
	m_values[Ky] = kBasis4D.y;
	m_values[Ty] = translation4D.y;

	m_values[Iz] = iBasis4D.z;
	m_values[Jz] = jBasis4D.z;
	m_values[Kz] = kBasis4D.z;
	m_values[Tz] = translation4D.z;

	m_values[Iw] = iBasis4D.w;
	m_values[Jw] = jBasis4D.w;
	m_values[Kw] = kBasis4D.w;
	m_values[Tw] = translation4D.w;
}


void Mat44::Transpose()
{
	Vec4 Itransposed(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]);
	Vec4 Jtransposed(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]);
	Vec4 Ktransposed(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]);
	Vec4 Ttransposed(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]);

	SetIJKT4D(Itransposed, Jtransposed, Ktransposed, Ttransposed);
}

void Mat44::Orthonormalize_IFwd_JLeft_KUp()
{
	//Gram–Schmidt process

	Vec3 iBasis = GetIBasis3D();
	Vec3 jBasis = GetJBasis3D();
	Vec3 kBasis = GetKBasis3D();

	float dot = DotProduct3D(iBasis, kBasis);
	iBasis -= dot * kBasis;
	iBasis.Normalize();

	float handedness =
		(iBasis.x * (kBasis.y * kBasis.z - kBasis.z * kBasis.y) +
			iBasis.y * (kBasis.z * kBasis.x - kBasis.x * kBasis.z) +
			iBasis.z * (kBasis.x * kBasis.y - kBasis.y * kBasis.x)) < 0.0f ? -1.0f : 1.0f;

	iBasis.z = handedness;
	jBasis = CrossProduct3D(kBasis, iBasis);

	SetIJK3D(iBasis, jBasis, kBasis);
}

void Mat44::Append(Mat44 const& appendThis)
{
	Vec4 LeftX = Vec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]);
	Vec4 LeftY = Vec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]);
	Vec4 LeftZ = Vec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]);
	Vec4 LeftW = Vec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]);

	Vec4 RightI = Vec4(appendThis.m_values[Ix], appendThis.m_values[Iy], appendThis.m_values[Iz], appendThis.m_values[Iw]);
	Vec4 RightJ = Vec4(appendThis.m_values[Jx], appendThis.m_values[Jy], appendThis.m_values[Jz], appendThis.m_values[Jw]);
	Vec4 RightK = Vec4(appendThis.m_values[Kx], appendThis.m_values[Ky], appendThis.m_values[Kz], appendThis.m_values[Kw]);
	Vec4 RightT = Vec4(appendThis.m_values[Tx], appendThis.m_values[Ty], appendThis.m_values[Tz], appendThis.m_values[Tw]);

	m_values[Ix] = DotProduct4D(LeftX, RightI);
	m_values[Iy] = DotProduct4D(LeftY, RightI);
	m_values[Iz] = DotProduct4D(LeftZ, RightI);
	m_values[Iw] = DotProduct4D(LeftW, RightI);

	m_values[Jx] = DotProduct4D(LeftX, RightJ);
	m_values[Jy] = DotProduct4D(LeftY, RightJ);
	m_values[Jz] = DotProduct4D(LeftZ, RightJ);
	m_values[Jw] = DotProduct4D(LeftW, RightJ);

	m_values[Kx] = DotProduct4D(LeftX, RightK);
	m_values[Ky] = DotProduct4D(LeftY, RightK);
	m_values[Kz] = DotProduct4D(LeftZ, RightK);
	m_values[Kw] = DotProduct4D(LeftW, RightK);

	m_values[Tx] = DotProduct4D(LeftX, RightT);
	m_values[Ty] = DotProduct4D(LeftY, RightT);
	m_values[Tz] = DotProduct4D(LeftZ, RightT);
	m_values[Tw] = DotProduct4D(LeftW, RightT);
}

void Mat44::AppendQuaternion(Quaternion const& quaternion)
{
	Quaternion q = quaternion;
	// Normalize the quaternion to ensure it represents a valid rotation
	q.Normalize();

	// Convert quaternion to rotation matrix
	float xx = q.i * q.i;
	float xy = q.i * q.j;
	float xz = q.i * q.k;
	float xw = q.i * q.w;
	float yy = q.j * q.j;
	float yz = q.j * q.k;
	float yw = q.j * q.w;
	float zz = q.k * q.k;
	float zw = q.k * q.w;

	Mat44 rotationMatrix;
	rotationMatrix.m_values[Ix] = 1 - 2 * (yy + zz);
	rotationMatrix.m_values[Iy] = 2 * (xy - zw);
	rotationMatrix.m_values[Iz] = 2 * (xz + yw);
	rotationMatrix.m_values[Iw] = 0.0f;

	rotationMatrix.m_values[Jx] = 2 * (xy + zw);
	rotationMatrix.m_values[Jy] = 1 - 2 * (xx + zz);
	rotationMatrix.m_values[Jz] = 2 * (yz - xw);
	rotationMatrix.m_values[Jw] = 0.0f;

	rotationMatrix.m_values[Kx] = 2 * (xz - yw);
	rotationMatrix.m_values[Ky] = 2 * (yz + xw);
	rotationMatrix.m_values[Kz] = 1 - 2 * (xx + yy);
	rotationMatrix.m_values[Kw] = 0.0f;

	rotationMatrix.m_values[Tx] = 0.0f;
	rotationMatrix.m_values[Ty] = 0.0f;
	rotationMatrix.m_values[Tz] = 0.0f;
	rotationMatrix.m_values[Tw] = 1.0f;

	// Append the rotation matrix to the current matrix
	Append(rotationMatrix);
}

void Mat44::RotateToQuaternion(Quaternion q)
{
	// Normalize the quaternion to ensure it represents a valid rotation
	q.Normalize();

	// Convert quaternion to rotation matrix
	float ii = q.i * q.i;
	float ij = q.i * q.j;
	float ik = q.i * q.k;
	float iw = q.i * q.w;
	float jj = q.j * q.j;
	float jk = q.j * q.k;
	float jw = q.j * q.w;
	float kk = q.k * q.k;
	float kw = q.k * q.w;

	// Set the rotation part of the matrix
	m_values[Ix] = 1 - 2 * (jj + kk);
	m_values[Iy] = 2 * (ij - kw);
	m_values[Iz] = 2 * (ik + jw);

	m_values[Jx] = 2 * (ij + kw);
	m_values[Jy] = 1 - 2 * (ii + kk);
	m_values[Jz] = 2 * (jk - iw);

	m_values[Kx] = 2 * (ik - jw);
	m_values[Ky] = 2 * (jk + iw);
	m_values[Kz] = 1 - 2 * (ii + jj);

	// Ensure the last row remains [0, 0, 0, 1]
	m_values[Iw] = 0.0f;
	m_values[Jw] = 0.0f;
	m_values[Kw] = 0.0f;
	m_values[Tw] = 1.0f;
}

void Mat44::AppendRotationAxis(float degreesRotation, const Vec3& rotationAxis)
{
	float radians = ConvertDegreesToRadians(degreesRotation);
	Vec3 axis = rotationAxis.GetNormalized();
	float c = cosf(radians);
	float s = sinf(radians);
	float t = 1.0f - c;

	float x = axis.x, y = axis.y, z = axis.z;

	Mat44 rotationMatrix;
	rotationMatrix.m_values[Ix] = t * x * x + c;
	rotationMatrix.m_values[Iy] = t * x * y - s * z;
	rotationMatrix.m_values[Iz] = t * x * z + s * y;
	rotationMatrix.m_values[Iw] = 0.0f;

	rotationMatrix.m_values[Jx] = t * x * y + s * z;
	rotationMatrix.m_values[Jy] = t * y * y + c;
	rotationMatrix.m_values[Jz] = t * y * z - s * x;
	rotationMatrix.m_values[Jw] = 0.0f;

	rotationMatrix.m_values[Kx] = t * x * z - s * y;
	rotationMatrix.m_values[Ky] = t * y * z + s * x;
	rotationMatrix.m_values[Kz] = t * z * z + c;
	rotationMatrix.m_values[Kw] = 0.0f;

	rotationMatrix.m_values[Tx] = 0.0f;
	rotationMatrix.m_values[Ty] = 0.0f;
	rotationMatrix.m_values[Tz] = 0.0f;
	rotationMatrix.m_values[Tw] = 1.0f;

	Append(rotationMatrix);
}

void Mat44::AppendZRotation(float degreesRotationAboutZ)
{
	Mat44 mat = Mat44::CreateZRotationDegrees(degreesRotationAboutZ);
	Append(mat);
}

void Mat44::AppendYRotation(float degreesRotationAboutY)
{
	Mat44 mat = Mat44::CreateYRotationDegrees(degreesRotationAboutY);
	Append(mat);
}

void Mat44::AppendXRotation(float degreesRotationAboutX)
{
	Mat44 mat = Mat44::CreateXRotationDegrees(degreesRotationAboutX);
	Append(mat);
}

void Mat44::AppendTranslation2D(Vec2 const& translationXY)
{
	Mat44 mat = Mat44::CreateTranslation2D(translationXY);
	Append(mat);
}

void Mat44::AppendTranslation3D(Vec3 const& translationXYZ)
{
	Mat44 mat = Mat44::CreateTranslation3D(translationXYZ);
	Append(mat);
}

void Mat44::AppendScaleUniform2D(float uniformScaleXY)
{
	Mat44 mat = Mat44::CreateUniformScale2D(uniformScaleXY);
	Append(mat);
}

void Mat44::AppendScaleUniform3D(float uniformScaleXYZ)
{
	Mat44 mat = Mat44::CreateUniformScale3D(uniformScaleXYZ);
	Append(mat);
}

void Mat44::AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY)
{
	Mat44 mat = Mat44::CreateNonUniformScale2D(nonUniformScaleXY);
	Append(mat);
}

void Mat44::AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ)
{
	Mat44 mat = Mat44::CreateNonUniformScale3D(nonUniformScaleXYZ);
	Append(mat);
}

bool Mat44::operator!=(const Mat44& compare) const
{
	return !(*this == compare);
}

bool Mat44::operator==(const Mat44& compare) const
{
	return
		m_values[Ix] == compare.m_values[Ix]
		&& m_values[Jx] == compare.m_values[Jx]
		&& m_values[Kx] == compare.m_values[Kx]
		&& m_values[Tx] == compare.m_values[Tx]
		&& m_values[Iy] == compare.m_values[Iy]
		&& m_values[Jy] == compare.m_values[Jy]
		&& m_values[Ky] == compare.m_values[Ky]
		&& m_values[Ty] == compare.m_values[Ty]
		&& m_values[Iz] == compare.m_values[Iz]
		&& m_values[Jz] == compare.m_values[Jz]
		&& m_values[Kz] == compare.m_values[Kz]
		&& m_values[Tz] == compare.m_values[Tz]
		&& m_values[Iw] == compare.m_values[Iw]
		&& m_values[Jw] == compare.m_values[Jw]
		&& m_values[Kw] == compare.m_values[Kw]
		&& m_values[Tw] == compare.m_values[Tw];
}
