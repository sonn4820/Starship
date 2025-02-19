#pragma once
#include "Engine/Math/DoubleVec2.hpp"
#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/DoubleVec4.hpp"
#include "Engine/Math/MathUtils.hpp"

struct DoubleQuaternion;

struct DoubleMat44
{
	enum
	{
		Ix, Iy, Iz, Iw,
		Jx, Jy, Jz, Jw,
		Kx, Ky, Kz, Kw,
		Tx, Ty, Tz, Tw
	};

	double m_values[16];

	DoubleMat44();
	DoubleMat44(Mat44 mat);

	static DoubleMat44 IDENTITY;

	explicit DoubleMat44(DoubleVec2 const& iBasis2D, DoubleVec2 const& jBasis2D, DoubleVec2 const& translation2D = DoubleVec2());
	explicit DoubleMat44(DoubleVec3 const& iBasis3D, DoubleVec3 const& jBasis3D, DoubleVec3 const& kBasis3D, DoubleVec3 const& translation3D = DoubleVec3());
	explicit DoubleMat44(DoubleVec4 const& iBasis4D, DoubleVec4 const& jBasis4D, DoubleVec4 const& kBasis4D, DoubleVec4 const& translation4D);
	explicit DoubleMat44(double const* sixteenValuesBasisMajor);

	static DoubleMat44 const CreateTranslation2D(DoubleVec2 const& translationXY);
	static DoubleMat44 const CreateTranslation3D(DoubleVec3 const& translationXYZ);
	static DoubleMat44 const CreateUniformScale2D(double uniformScaleXY);
	static DoubleMat44 const CreateUniformScale3D(double uniformScaleXYZ);
	static DoubleMat44 const CreateNonUniformScale2D(DoubleVec2 const& nonUniformScaleXY);
	static DoubleMat44 const CreateNonUniformScale3D(DoubleVec3 const& nonUniformScaleXYZ);
	static DoubleMat44 const CreateZRotationDegrees(double rotationDegreesAboutZ);
	static DoubleMat44 const CreateYRotationDegrees(double rotationDegreesAboutY);
	static DoubleMat44 const CreateXRotationDegrees(double rotationDegreesAboutX);
	static DoubleMat44 const CreateOrthoProjection(double left, double right, double bottom, double top, double zNear, double zFar);
	static DoubleMat44 const CreatePerspectiveProjection(double fovYDegrees, double aspect, double zNear, double zFar);
	static DoubleMat44 const CreateLookForward(DoubleVec3 const& iBasis);
	static DoubleMat44 const TransformWorldToLocal(DoubleMat44 const& worldMat);

	DoubleVec2 const	TransformVectorQuantity2D(DoubleVec2 const& vectorQuantityXY) const;
	DoubleVec3 const	TransformVectorQuantity3D(DoubleVec3 const& vectorQuantityXYZ) const;
	DoubleVec2 const	TransformPosition2D(DoubleVec2 const& positionXY) const;
	DoubleVec3 const	TransformPosition3D(DoubleVec3 const& positionXYZ) const;
	DoubleVec4 const	TransformHomogeneous3D(DoubleVec4 const& homogeneousPoint3D) const;

	double* GetAsdoubleArray();
	double const* GetAsdoubleArray() const;
	DoubleVec2			GetIBasis2D() const;
	DoubleVec2			GetJBasis2D() const;
	DoubleVec2			GetTranslation2D() const;
	DoubleVec3			GetIBasis3D() const;
	DoubleVec3			GetJBasis3D() const;
	DoubleVec3			GetKBasis3D() const;
	DoubleVec3			GetTranslation3D() const;
	DoubleVec4			GetIBasis4D() const;
	DoubleVec4			GetJBasis4D() const;
	DoubleVec4			GetKBasis4D() const;
	DoubleVec4			GetTranslation4D() const;
	DoubleMat44			GetOrthonormalInverse() const;
	DoubleMat44			GetLookAtTarget(DoubleVec3 const& targetPosition) const;
	EulerAngles			GetEulerAngle() const;
	DoubleQuaternion	GetDoubleQuaternion() const;
	DoubleMat44			GetInverseRotationMatrix() const;

	void LookAtTarget(DoubleVec3 const& targetPosition);
	void LookAtTargetXY(DoubleVec3 const& targetPosition);
	void SetTranslation2D(DoubleVec2 const& translationXY);
	void SetTranslation3D(DoubleVec3 const& translationXYZ);
	void SetDoubleQuaternion(DoubleQuaternion q);
	void SetEulerAngle(EulerAngles q);
	void SetIJ2D(DoubleVec2 const& iBasis2D, DoubleVec2 const& jBasis2D);
	void SetIJT2D(DoubleVec2 const& iBasis2D, DoubleVec2 const& jBasis2D, DoubleVec2 const& translationXY);
	void SetIJK3D(DoubleVec3 const& iBasis3D, DoubleVec3 const& jBasis3D, DoubleVec3 const& kBasis3D);
	void SetIJKT3D(DoubleVec3 const& iBasis3D, DoubleVec3 const& jBasis3D, DoubleVec3 const& kBasis3D, DoubleVec3 const& translationXYZ);
	void SetIJKT4D(DoubleVec4 const& iBasis4D, DoubleVec4 const& jBasis4D, DoubleVec4 const& kBasis4D, DoubleVec4 const& translation4D);
	void Transpose();
	void Orthonormalize_IFwd_JLeft_KUp();

	void Append(DoubleMat44 const& appendThis);
	void AppendDoubleQuaternion(DoubleQuaternion const& q);
	void RotateToDoubleQuaternion(DoubleQuaternion q);
	void AppendRotationAxis(double degreesRotation, const DoubleVec3& rotationAxis);
	void AppendZRotation(double degreesRotationAboutZ);
	void AppendYRotation(double degreesRotationAboutY);
	void AppendXRotation(double degreesRotationAboutX);
	void AppendTranslation2D(DoubleVec2 const& translationXY);
	void AppendTranslation3D(DoubleVec3 const& translationXYZ);
	void AppendScaleUniform2D(double uniformScaleXY);
	void AppendScaleUniform3D(double uniformScaleXYZ);
	void AppendScaleNonUniform2D(DoubleVec2 const& nonUniformScaleXY);
	void AppendScaleNonUniform3D(DoubleVec3 const& nonUniformScaleXYZ);

	bool		operator==(const DoubleMat44& compare) const;
	bool		operator!=(const DoubleMat44& compare) const;



};


