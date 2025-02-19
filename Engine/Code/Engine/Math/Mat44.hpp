#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec4.hpp"
#include "Engine/Math/MathUtils.hpp"

struct Quaternion;
struct DoubleMat44;

struct Mat44
{
	enum
	{
		Ix, Iy, Iz, Iw,
		Jx, Jy, Jz, Jw,
		Kx, Ky, Kz, Kw,
		Tx, Ty, Tz, Tw
	};

	float m_values[16];

	Mat44();
	Mat44(DoubleMat44 dmat);
	explicit Mat44(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translation2D = Vec2());
	explicit Mat44(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translation3D = Vec3());
	explicit Mat44(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D);
	explicit Mat44(float const* sixteenValuesBasisMajor);

	static Mat44 const CreateTranslation2D(Vec2 const& translationXY);
	static Mat44 const CreateTranslation3D(Vec3 const& translationXYZ);
	static Mat44 const CreateUniformScale2D(float uniformScaleXY);
	static Mat44 const CreateUniformScale3D(float uniformScaleXYZ);
	static Mat44 const CreateNonUniformScale2D(Vec2 const& nonUniformScaleXY);
	static Mat44 const CreateNonUniformScale3D(Vec3 const& nonUniformScaleXYZ);
	static Mat44 const CreateZRotationDegrees(float rotationDegreesAboutZ);
	static Mat44 const CreateYRotationDegrees(float rotationDegreesAboutY);
	static Mat44 const CreateXRotationDegrees(float rotationDegreesAboutX);
	static Mat44 const CreateOrthoProjection(float left, float right, float bottom, float top, float zNear, float zFar);
	static Mat44 const CreatePerspectiveProjection(float fovYDegrees, float aspect, float zNear, float zFar);
	static Mat44 const CreateLookForward(Vec3 const& iBasis);
	static Mat44 const TransformWorldToLocal(Mat44 const& worldMat);

	Vec2 const	TransformVectorQuantity2D(Vec2 const& vectorQuantityXY) const;
	Vec3 const	TransformVectorQuantity3D(Vec3 const& vectorQuantityXYZ) const;
	Vec2 const	TransformPosition2D(Vec2 const& positionXY) const;
	Vec3 const	TransformPosition3D(Vec3 const& positionXYZ) const;
	Vec4 const	TransformHomogeneous3D(Vec4 const& homogeneousPoint3D) const;

	float* GetAsFloatArray();
	float const* GetAsFloatArray() const;
	Vec2			GetIBasis2D() const;
	Vec2			GetJBasis2D() const;
	Vec2			GetTranslation2D() const;
	Vec3			GetIBasis3D() const;
	Vec3			GetJBasis3D() const;
	Vec3			GetKBasis3D() const;
	Vec3			GetTranslation3D() const;
	Vec4			GetIBasis4D() const;
	Vec4			GetJBasis4D() const;
	Vec4			GetKBasis4D() const;
	Vec4			GetTranslation4D() const;
	Mat44			GetOrthonormalInverse() const;
	Mat44			GetLookAtTarget(Vec3 const& targetPosition) const;
	EulerAngles  GetEulerAngle() const;
	Quaternion  GetQuaternion() const;
	Mat44 GetInverseRotationMatrix() const;

	void LookAtTarget(Vec3 const& targetPosition);
	void LookAtTargetXY(Vec3 const& targetPosition);
	void SetTranslation2D(Vec2 const& translationXY);
	void SetTranslation3D(Vec3 const& translationXYZ);
	void SetQuaternion(Quaternion q);
	void SetEulerAngle(EulerAngles q);
	void SetIJ2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D);
	void SetIJT2D(Vec2 const& iBasis2D, Vec2 const& jBasis2D, Vec2 const& translationXY);
	void SetIJK3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D);
	void SetIJKT3D(Vec3 const& iBasis3D, Vec3 const& jBasis3D, Vec3 const& kBasis3D, Vec3 const& translationXYZ);
	void SetIJKT4D(Vec4 const& iBasis4D, Vec4 const& jBasis4D, Vec4 const& kBasis4D, Vec4 const& translation4D);
	void Transpose();
	void Orthonormalize_IFwd_JLeft_KUp();

	void Append(Mat44 const& appendThis);
	void AppendQuaternion(Quaternion const& q);
	void RotateToQuaternion(Quaternion q);
	void AppendRotationAxis(float degreesRotation, const Vec3& rotationAxis);
	void AppendZRotation(float degreesRotationAboutZ);
	void AppendYRotation(float degreesRotationAboutY);
	void AppendXRotation(float degreesRotationAboutX);
	void AppendTranslation2D(Vec2 const& translationXY);
	void AppendTranslation3D(Vec3 const& translationXYZ);
	void AppendScaleUniform2D(float uniformScaleXY);
	void AppendScaleUniform3D(float uniformScaleXYZ);
	void AppendScaleNonUniform2D(Vec2 const& nonUniformScaleXY);
	void AppendScaleNonUniform3D(Vec3 const& nonUniformScaleXYZ);

	bool		operator==(const Mat44& compare) const;
	bool		operator!=(const Mat44& compare) const;
};


