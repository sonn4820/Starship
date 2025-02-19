#include "Engine/Math/DoubleMat44.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Math/DoubleQuaternion.hpp"
#include "Engine/Math/MathUtils.hpp"
#include <math.h>

DoubleMat44 DoubleMat44::IDENTITY = DoubleMat44();

DoubleMat44::DoubleMat44()
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

DoubleMat44::DoubleMat44(DoubleVec2 const& iBasis2D, DoubleVec2 const& jBasis2D, DoubleVec2 const& translation2D)
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

DoubleMat44::DoubleMat44(DoubleVec3 const& iBasis3D, DoubleVec3 const& jBasis3D, DoubleVec3 const& kBasis3D, DoubleVec3 const& translation3D)
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

DoubleMat44::DoubleMat44(DoubleVec4 const& iBasis4D, DoubleVec4 const& jBasis4D, DoubleVec4 const& kBasis4D, DoubleVec4 const& translation4D)
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

DoubleMat44::DoubleMat44(double const* sixteenValuesBasisMajor)
{
	for (int i = 0; i < 16; i++)
	{
		m_values[i] = sixteenValuesBasisMajor[i];
	}
}

DoubleMat44::DoubleMat44(Mat44 mat)
{
	m_values[Ix] = static_cast<double>(mat.m_values[Ix]);
	m_values[Jx] = static_cast<double>(mat.m_values[Jx]);
	m_values[Kx] = static_cast<double>(mat.m_values[Kx]);
	m_values[Tx] = static_cast<double>(mat.m_values[Tx]);
	
	m_values[Iy] = static_cast<double>(mat.m_values[Iy]);
	m_values[Jy] = static_cast<double>(mat.m_values[Jy]);
	m_values[Ky] = static_cast<double>(mat.m_values[Ky]);
	m_values[Ty] = static_cast<double>(mat.m_values[Ty]);
	
	m_values[Iz] = static_cast<double>(mat.m_values[Iz]);
	m_values[Jz] = static_cast<double>(mat.m_values[Jz]);
	m_values[Kz] = static_cast<double>(mat.m_values[Kz]);
	m_values[Tz] = static_cast<double>(mat.m_values[Tz]);
	
	m_values[Iw] = static_cast<double>(mat.m_values[Iw]);
	m_values[Jw] = static_cast<double>(mat.m_values[Jw]);
	m_values[Kw] = static_cast<double>(mat.m_values[Kw]);
	m_values[Tw] = static_cast<double>(mat.m_values[Tw]);
}

DoubleMat44 const DoubleMat44::CreateTranslation2D(DoubleVec2 const& translationXY)
{
	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Tx] = translationXY.x;
	mat.m_values[Ty] = translationXY.y;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateTranslation3D(DoubleVec3 const& translationXYZ)
{
	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Tx] = translationXYZ.x;
	mat.m_values[Ty] = translationXYZ.y;
	mat.m_values[Tz] = translationXYZ.z;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateUniformScale2D(double uniformScaleXY)
{
	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Ix] = uniformScaleXY;
	mat.m_values[Jy] = uniformScaleXY;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateUniformScale3D(double uniformScaleXYZ)
{
	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Ix] = uniformScaleXYZ;
	mat.m_values[Jy] = uniformScaleXYZ;
	mat.m_values[Kz] = uniformScaleXYZ;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateNonUniformScale2D(DoubleVec2 const& nonUniformScaleXY)
{
	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Ix] = nonUniformScaleXY.x;
	mat.m_values[Jy] = nonUniformScaleXY.y;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateNonUniformScale3D(DoubleVec3 const& nonUniformScaleXYZ)
{
	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Ix] = nonUniformScaleXYZ.x;
	mat.m_values[Jy] = nonUniformScaleXYZ.y;
	mat.m_values[Kz] = nonUniformScaleXYZ.z;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateZRotationDegrees(double rotationDegreesAboutZ)
{
	double cosAngle = CosDegreesDouble(rotationDegreesAboutZ);
	double sinAngle = SinDegreesDouble(rotationDegreesAboutZ);

	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Ix] = cosAngle;
	mat.m_values[Iy] = sinAngle;
	mat.m_values[Jx] = -sinAngle;
	mat.m_values[Jy] = cosAngle;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateYRotationDegrees(double rotationDegreesAboutY)
{
	double cosAngle = CosDegreesDouble(rotationDegreesAboutY);
	double sinAngle = SinDegreesDouble(rotationDegreesAboutY);

	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Ix] = cosAngle;
	mat.m_values[Iz] = -sinAngle;
	mat.m_values[Kx] = sinAngle;
	mat.m_values[Kz] = cosAngle;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateXRotationDegrees(double rotationDegreesAboutX)
{
	double cosAngle = CosDegreesDouble(rotationDegreesAboutX);
	double sinAngle = SinDegreesDouble(rotationDegreesAboutX);

	DoubleMat44 mat = DoubleMat44();
	mat.m_values[Jy] = cosAngle;
	mat.m_values[Jz] = sinAngle;
	mat.m_values[Ky] = -sinAngle;
	mat.m_values[Kz] = cosAngle;
	return mat;
}

DoubleMat44 const DoubleMat44::CreateOrthoProjection(double left, double right, double bottom, double top, double zNear, double zFar)
{
	DoubleMat44 ortho = DoubleMat44();
	ortho.m_values[Ix] = 2.f / (right - left);
	ortho.m_values[Jy] = 2.f / (top - bottom);
	ortho.m_values[Kz] = 1.f / (zFar - zNear);
	ortho.m_values[Tx] = (left + right) / (left - right);
	ortho.m_values[Ty] = (bottom + top) / (bottom - top);
	ortho.m_values[Tz] = zNear / (zNear - zFar);
	return ortho;
}

DoubleMat44 const DoubleMat44::CreatePerspectiveProjection(double fovYDegrees, double aspect, double zNear, double zFar)
{
	DoubleMat44 perspective = DoubleMat44();

	double radFov = ConvertDegreesToRadiansDouble(fovYDegrees);
	double focalLength = 1.f / tan(radFov / 2.f);
	double zRange = zFar - zNear;

	perspective.m_values[Ix] = focalLength / aspect;
	perspective.m_values[Jy] = focalLength;
	perspective.m_values[Kz] = zFar / zRange;
	perspective.m_values[Kw] = 1.f;

	perspective.m_values[Tz] = -(zFar * zNear) / zRange;
	perspective.m_values[Tw] = 0.f;

	return perspective;
}

DoubleMat44 const DoubleMat44::CreateLookForward(DoubleVec3 const& iBasis)
{
	DoubleMat44 lookAt = DoubleMat44();

	DoubleVec3 kBasis;
	DoubleVec3 jBasis;

	if (fabs(DotProduct3D(iBasis, DoubleVec3(0.f, 0.f, 1.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(DoubleVec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, DoubleVec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	lookAt.SetIJK3D(iBasis, jBasis, kBasis);

	return lookAt;
}

DoubleMat44 const DoubleMat44::TransformWorldToLocal(DoubleMat44 const& worldMat)
{
	DoubleMat44 localMat = worldMat;
	localMat.GetOrthonormalInverse();
	return localMat;
}

DoubleVec2 const DoubleMat44::TransformVectorQuantity2D(DoubleVec2 const& vectorQuantityXY) const
{
	return (GetIBasis2D() * vectorQuantityXY.x) + (GetJBasis2D() * vectorQuantityXY.y);
}

DoubleVec3 const DoubleMat44::TransformVectorQuantity3D(DoubleVec3 const& vectorQuantityXYZ) const
{
	return (GetIBasis3D() * vectorQuantityXYZ.x) + (GetJBasis3D() * vectorQuantityXYZ.y) + (GetKBasis3D() * vectorQuantityXYZ.z);
}

DoubleVec2 const DoubleMat44::TransformPosition2D(DoubleVec2 const& positionXY) const
{
	return (GetIBasis2D() * positionXY.x) + (GetJBasis2D() * positionXY.y) + GetTranslation2D();
}

DoubleVec3 const DoubleMat44::TransformPosition3D(DoubleVec3 const& positionXYZ) const
{
	return (GetIBasis3D() * positionXYZ.x) + (GetJBasis3D() * positionXYZ.y) + (GetKBasis3D() * positionXYZ.z) + GetTranslation3D();
}

DoubleVec4 const DoubleMat44::TransformHomogeneous3D(DoubleVec4 const& homogeneousPoint3D) const
{
	return (GetIBasis4D() * homogeneousPoint3D.x) + (GetJBasis4D() * homogeneousPoint3D.y) + (GetKBasis4D() * homogeneousPoint3D.z) + (GetTranslation4D() * homogeneousPoint3D.w);
}

double* DoubleMat44::GetAsdoubleArray()
{
	return m_values;
}

double const* DoubleMat44::GetAsdoubleArray() const
{
	return m_values;
}

DoubleVec2 DoubleMat44::GetIBasis2D() const
{
	return DoubleVec2(m_values[Ix], m_values[Iy]);
}

DoubleVec2 DoubleMat44::GetJBasis2D() const
{
	return DoubleVec2(m_values[Jx], m_values[Jy]);
}

DoubleVec2 DoubleMat44::GetTranslation2D() const
{
	return DoubleVec2(m_values[Tx], m_values[Ty]);
}

DoubleVec3 DoubleMat44::GetIBasis3D() const
{
	return DoubleVec3(m_values[Ix], m_values[Iy], m_values[Iz]);
}

DoubleVec3 DoubleMat44::GetJBasis3D() const
{
	return DoubleVec3(m_values[Jx], m_values[Jy], m_values[Jz]);
}

DoubleVec3 DoubleMat44::GetKBasis3D() const
{
	return DoubleVec3(m_values[Kx], m_values[Ky], m_values[Kz]);
}

DoubleVec3 DoubleMat44::GetTranslation3D() const
{
	return DoubleVec3(m_values[Tx], m_values[Ty], m_values[Tz]);
}

DoubleVec4 DoubleMat44::GetIBasis4D() const
{
	return DoubleVec4(m_values[Ix], m_values[Iy], m_values[Iz], m_values[Iw]);
}

DoubleVec4 DoubleMat44::GetJBasis4D() const
{
	return DoubleVec4(m_values[Jx], m_values[Jy], m_values[Jz], m_values[Jw]);
}

DoubleVec4 DoubleMat44::GetKBasis4D() const
{
	return DoubleVec4(m_values[Kx], m_values[Ky], m_values[Kz], m_values[Kw]);
}

DoubleVec4 DoubleMat44::GetTranslation4D() const
{
	return DoubleVec4(m_values[Tx], m_values[Ty], m_values[Tz], m_values[Tw]);
}

DoubleMat44 DoubleMat44::GetOrthonormalInverse() const
{
	DoubleMat44 antiRotation;
	antiRotation.SetIJK3D(GetIBasis3D(), GetJBasis3D(), GetKBasis3D());
	antiRotation.Transpose();

	DoubleMat44 antiTranslation;
	antiTranslation.SetTranslation3D(-1.f * GetTranslation3D());

	DoubleMat44 result = antiRotation;
	result.Append(antiTranslation);

	return result;
}

DoubleMat44 DoubleMat44::GetLookAtTarget(DoubleVec3 const& targetPosition) const
{
	DoubleMat44 result;

	DoubleVec3 axis = targetPosition - GetTranslation3D();
	DoubleVec3 iBasis = axis.GetNormalized();
	DoubleVec3 kBasis;
	DoubleVec3 jBasis;

	if (fabs(DotProduct3D(iBasis, DoubleVec3(0.f, 0.f, 1.f))) < 0.9999f)
	{
		jBasis = CrossProduct3D(DoubleVec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, DoubleVec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	result.SetIJK3D(iBasis, jBasis, kBasis);
	return result;
}

EulerAngles DoubleMat44::GetEulerAngle() const
{
	EulerAngles result;
	if (m_values[Kz] < 1)
	{
		if (m_values[Kz] > -1)
		{
			result.m_pitchDegrees = static_cast<float>(asin(m_values[Kz]));
			result.m_yawDegrees = static_cast<float>(atan2(-m_values[Jz], m_values[Iz]));
			result.m_rollDegrees = static_cast<float>(atan2(-m_values[Ky], m_values[Kx]));
		}
		else
		{
			result.m_pitchDegrees = -PI / 2;
			result.m_yawDegrees = -static_cast<float>(atan2(m_values[Jy], m_values[Jx]));
			result.m_rollDegrees = 0.f;
		}
	}
	else
	{
		result.m_pitchDegrees = PI / 2;
		result.m_yawDegrees = static_cast<float>(atan2(m_values[Jy], m_values[Jx]));
		result.m_rollDegrees = 0.f;
	}

	result.m_pitchDegrees = ConvertRadiansToDegrees(result.m_pitchDegrees);
	result.m_yawDegrees = ConvertRadiansToDegrees(result.m_yawDegrees);
	result.m_rollDegrees = ConvertRadiansToDegrees(result.m_rollDegrees);

	return result;
}

DoubleQuaternion DoubleMat44::GetDoubleQuaternion() const
{
	return DoubleQuaternion(*this);
}

DoubleMat44 DoubleMat44::GetInverseRotationMatrix() const
{
	DoubleMat44 result;
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

void DoubleMat44::LookAtTarget(DoubleVec3 const& targetPosition)
{
	DoubleVec3 axis = targetPosition - GetTranslation3D();
	DoubleVec3 iBasis = axis.GetNormalized();
	DoubleVec3 kBasis;
	DoubleVec3 jBasis;

	if (fabs(DotProduct3D(iBasis, DoubleVec3(0.f, 0.f, 1.f))) < 0.9999f)
	{
		jBasis = CrossProduct3D(DoubleVec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, DoubleVec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	SetIJK3D(iBasis, jBasis, kBasis);
}

void DoubleMat44::LookAtTargetXY(DoubleVec3 const& targetPosition)
{
	DoubleVec3 axis = targetPosition - GetTranslation3D();
	DoubleVec3 iBasis = axis.GetNormalized();
	DoubleVec3 kBasis;
	DoubleVec3 jBasis;

	if (fabs(DotProduct3D(iBasis, DoubleVec3(0.f, 0.f, 1.f))) != 1.f)
	{
		jBasis = CrossProduct3D(DoubleVec3(0.f, 0.f, 1.f), iBasis).GetNormalized();
		kBasis = CrossProduct3D(iBasis, jBasis);
	}
	else
	{
		kBasis = CrossProduct3D(iBasis, DoubleVec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}
	SetIJK3D(iBasis, jBasis, DoubleVec3(0.f, 0.f, 1.f));
}

void DoubleMat44::SetTranslation2D(DoubleVec2 const& translationXY)
{
	m_values[Tx] = translationXY.x;
	m_values[Ty] = translationXY.y;
	m_values[Tz] = 0;
	m_values[Tw] = 1;
}

void DoubleMat44::SetTranslation3D(DoubleVec3 const& translationXYZ)
{
	m_values[Tx] = translationXYZ.x;
	m_values[Ty] = translationXYZ.y;
	m_values[Tz] = translationXYZ.z;
	m_values[Tw] = 1;
}

void DoubleMat44::SetDoubleQuaternion(DoubleQuaternion q)
{
	*this = q.GetMatrix(this->GetTranslation3D());
}

void DoubleMat44::SetEulerAngle(EulerAngles a)
{
	double Cy = CosDegreesDouble(a.m_yawDegrees);
	double Sy = SinDegreesDouble(a.m_yawDegrees);
	double Cp = CosDegreesDouble(a.m_pitchDegrees);
	double Sp = SinDegreesDouble(a.m_pitchDegrees);
	double Cr = CosDegreesDouble(a.m_rollDegrees);
	double Sr = SinDegreesDouble(a.m_rollDegrees);

	DoubleVec3 forwardIBasis = DoubleVec3(Cy * Cp, Cp * Sy, -Sp);
	DoubleVec3 leftJBasis = DoubleVec3(Sr * Sp * Cy - Sy * Cr, Sr * Sp * Sy + Cr * Cy, Sr * Cp);
	DoubleVec3 upKBasis = DoubleVec3(Sp * Cr * Cy + Sy * Sr, Sp * Cr * Sy - Sr * Cy, Cp * Cr);

	SetIJK3D(forwardIBasis, leftJBasis, upKBasis);
}

void DoubleMat44::SetIJ2D(DoubleVec2 const& iBasis2D, DoubleVec2 const& jBasis2D)
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

void DoubleMat44::SetIJT2D(DoubleVec2 const& iBasis2D, DoubleVec2 const& jBasis2D, DoubleVec2 const& translationXY)
{
	SetIJ2D(iBasis2D, jBasis2D);
	SetTranslation2D(translationXY);
}

void DoubleMat44::SetIJK3D(DoubleVec3 const& iBasis3D, DoubleVec3 const& jBasis3D, DoubleVec3 const& kBasis3D)
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

void DoubleMat44::SetIJKT3D(DoubleVec3 const& iBasis3D, DoubleVec3 const& jBasis3D, DoubleVec3 const& kBasis3D, DoubleVec3 const& translationXYZ)
{
	SetIJK3D(iBasis3D, jBasis3D, kBasis3D);
	SetTranslation3D(translationXYZ);
}

void DoubleMat44::SetIJKT4D(DoubleVec4 const& iBasis4D, DoubleVec4 const& jBasis4D, DoubleVec4 const& kBasis4D, DoubleVec4 const& translation4D)
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


void DoubleMat44::Transpose()
{
	DoubleVec4 Itransposed(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]);
	DoubleVec4 Jtransposed(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]);
	DoubleVec4 Ktransposed(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]);
	DoubleVec4 Ttransposed(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]);

	SetIJKT4D(Itransposed, Jtransposed, Ktransposed, Ttransposed);
}

void DoubleMat44::Orthonormalize_IFwd_JLeft_KUp()
{
	//Gram–Schmidt process

	DoubleVec3 iBasis = GetIBasis3D();
	DoubleVec3 jBasis = GetJBasis3D();
	DoubleVec3 kBasis = GetKBasis3D();

	double dot = DotProduct3D(iBasis, kBasis);
	iBasis -= dot * kBasis;
	iBasis.Normalize();

	double handedness =
		(iBasis.x * (kBasis.y * kBasis.z - kBasis.z * kBasis.y) +
			iBasis.y * (kBasis.z * kBasis.x - kBasis.x * kBasis.z) +
			iBasis.z * (kBasis.x * kBasis.y - kBasis.y * kBasis.x)) < 0.0f ? -1.0f : 1.0f;

	iBasis.z = handedness;
	jBasis = CrossProduct3D(kBasis, iBasis);

	SetIJK3D(iBasis, jBasis, kBasis);
}

void DoubleMat44::Append(DoubleMat44 const& appendThis)
{
	DoubleVec4 LeftX = DoubleVec4(m_values[Ix], m_values[Jx], m_values[Kx], m_values[Tx]);
	DoubleVec4 LeftY = DoubleVec4(m_values[Iy], m_values[Jy], m_values[Ky], m_values[Ty]);
	DoubleVec4 LeftZ = DoubleVec4(m_values[Iz], m_values[Jz], m_values[Kz], m_values[Tz]);
	DoubleVec4 LeftW = DoubleVec4(m_values[Iw], m_values[Jw], m_values[Kw], m_values[Tw]);

	DoubleVec4 RightI = DoubleVec4(appendThis.m_values[Ix], appendThis.m_values[Iy], appendThis.m_values[Iz], appendThis.m_values[Iw]);
	DoubleVec4 RightJ = DoubleVec4(appendThis.m_values[Jx], appendThis.m_values[Jy], appendThis.m_values[Jz], appendThis.m_values[Jw]);
	DoubleVec4 RightK = DoubleVec4(appendThis.m_values[Kx], appendThis.m_values[Ky], appendThis.m_values[Kz], appendThis.m_values[Kw]);
	DoubleVec4 RightT = DoubleVec4(appendThis.m_values[Tx], appendThis.m_values[Ty], appendThis.m_values[Tz], appendThis.m_values[Tw]);

	m_values[Ix] = DotProduct4D_Double(LeftX, RightI);
	m_values[Iy] = DotProduct4D_Double(LeftY, RightI);
	m_values[Iz] = DotProduct4D_Double(LeftZ, RightI);
	m_values[Iw] = DotProduct4D_Double(LeftW, RightI);

	m_values[Jx] = DotProduct4D_Double(LeftX, RightJ);
	m_values[Jy] = DotProduct4D_Double(LeftY, RightJ);
	m_values[Jz] = DotProduct4D_Double(LeftZ, RightJ);
	m_values[Jw] = DotProduct4D_Double(LeftW, RightJ);

	m_values[Kx] = DotProduct4D_Double(LeftX, RightK);
	m_values[Ky] = DotProduct4D_Double(LeftY, RightK);
	m_values[Kz] = DotProduct4D_Double(LeftZ, RightK);
	m_values[Kw] = DotProduct4D_Double(LeftW, RightK);

	m_values[Tx] = DotProduct4D_Double(LeftX, RightT);
	m_values[Ty] = DotProduct4D_Double(LeftY, RightT);
	m_values[Tz] = DotProduct4D_Double(LeftZ, RightT);
	m_values[Tw] = DotProduct4D_Double(LeftW, RightT);
}

void DoubleMat44::AppendDoubleQuaternion(DoubleQuaternion const& quaternion)
{
	DoubleQuaternion q = quaternion;
	// Normalize the quaternion to ensure it represents a valid rotation
	q.Normalize();

	// Convert DoubleQuaternion to rotation matrix
	double xx = q.i * q.i;
	double xy = q.i * q.j;
	double xz = q.i * q.k;
	double xw = q.i * q.w;
	double yy = q.j * q.j;
	double yz = q.j * q.k;
	double yw = q.j * q.w;
	double zz = q.k * q.k;
	double zw = q.k * q.w;

	DoubleMat44 rotationMatrix;
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

void DoubleMat44::RotateToDoubleQuaternion(DoubleQuaternion q)
{
	// Normalize the DoubleQuaternion to ensure it represents a valid rotation
	q.Normalize();

	// Convert DoubleQuaternion to rotation matrix
	double ii = q.i * q.i;
	double ij = q.i * q.j;
	double ik = q.i * q.k;
	double iw = q.i * q.w;
	double jj = q.j * q.j;
	double jk = q.j * q.k;
	double jw = q.j * q.w;
	double kk = q.k * q.k;
	double kw = q.k * q.w;

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

void DoubleMat44::AppendRotationAxis(double degreesRotation, const DoubleVec3& rotationAxis)
{
	double radians = ConvertDegreesToRadiansDouble(degreesRotation);
	DoubleVec3 axis = rotationAxis.GetNormalized();
	double c = cos(radians);
	double s = sin(radians);
	double t = 1.0f - c;

	double x = axis.x, y = axis.y, z = axis.z;

	DoubleMat44 rotationMatrix;
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

void DoubleMat44::AppendZRotation(double degreesRotationAboutZ)
{
	DoubleMat44 mat = DoubleMat44::CreateZRotationDegrees(degreesRotationAboutZ);
	Append(mat);
}

void DoubleMat44::AppendYRotation(double degreesRotationAboutY)
{
	DoubleMat44 mat = DoubleMat44::CreateYRotationDegrees(degreesRotationAboutY);
	Append(mat);
}

void DoubleMat44::AppendXRotation(double degreesRotationAboutX)
{
	DoubleMat44 mat = DoubleMat44::CreateXRotationDegrees(degreesRotationAboutX);
	Append(mat);
}

void DoubleMat44::AppendTranslation2D(DoubleVec2 const& translationXY)
{
	DoubleMat44 mat = DoubleMat44::CreateTranslation2D(translationXY);
	Append(mat);
}

void DoubleMat44::AppendTranslation3D(DoubleVec3 const& translationXYZ)
{
	DoubleMat44 mat = DoubleMat44::CreateTranslation3D(translationXYZ);
	Append(mat);
}

void DoubleMat44::AppendScaleUniform2D(double uniformScaleXY)
{
	DoubleMat44 mat = DoubleMat44::CreateUniformScale2D(uniformScaleXY);
	Append(mat);
}

void DoubleMat44::AppendScaleUniform3D(double uniformScaleXYZ)
{
	DoubleMat44 mat = DoubleMat44::CreateUniformScale3D(uniformScaleXYZ);
	Append(mat);
}

void DoubleMat44::AppendScaleNonUniform2D(DoubleVec2 const& nonUniformScaleXY)
{
	DoubleMat44 mat = DoubleMat44::CreateNonUniformScale2D(nonUniformScaleXY);
	Append(mat);
}

void DoubleMat44::AppendScaleNonUniform3D(DoubleVec3 const& nonUniformScaleXYZ)
{
	DoubleMat44 mat = DoubleMat44::CreateNonUniformScale3D(nonUniformScaleXYZ);
	Append(mat);
}

bool DoubleMat44::operator!=(const DoubleMat44& compare) const
{
	return !(*this == compare);
}

bool DoubleMat44::operator==(const DoubleMat44& compare) const
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
