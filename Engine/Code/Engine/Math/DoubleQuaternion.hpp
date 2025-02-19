#pragma once
#include "Engine/Math/DoubleVec3.hpp"
#include "Engine/Math/DoubleMat44.hpp"
#include "Engine/Math/EulerAngles.hpp"

//SOURCE: https://irrlicht.sourceforge.io/docu/DoubleQuaternion_8h_source.html?fbclid=IwZXh0bgNhZW0CMTAAAR0kwVfx5jXQVhbodQtewKJ7pS6S5nOY70o7VwNy8G6oZdYyk02OOgOIe2Q_aem_ARWr5VLrwj54SUNrwqcZvP9zoPe_SEQuLk74XPXsfeWV7bLR0cXeqPPBzLlVjOuCQvRbiLtGhMTOgLboO3NNDQxf
struct Quaternion;
struct DoubleQuaternion
{
public:
	DoubleQuaternion() = default;
	DoubleQuaternion(Quaternion q);
	DoubleQuaternion(DoubleQuaternion const& copyFrom);
	DoubleQuaternion(double i, double j, double k);
	DoubleQuaternion(double i, double j, double k, double w);
	explicit DoubleQuaternion(EulerAngles const& eulerAngle);
	explicit DoubleQuaternion(DoubleMat44 const& mat);
	explicit DoubleQuaternion(DoubleVec3 const& rotationVec);
	DoubleQuaternion(double angle, DoubleVec3 const& axis);

	DoubleMat44					GetMatrix(DoubleVec3 translation = DoubleVec3()) const;
	DoubleQuaternion			GetNormalized() const;
	void						Normalize();
	EulerAngles					ToEuler();
	void						ToAngleAxis(double& out_angleRadian, DoubleVec3& out_axis) const;

	bool						Equal(DoubleQuaternion const& q, double tolerance = 0.005f);
	double						DotProduct(DoubleQuaternion const& q) const;
	double						GetAngle();
	DoubleVec3					Rotate(const DoubleVec3& v) const;

	double						GetMagnitude() const;
	void						Conjugate();
	DoubleQuaternion			GetConjugated() const;
	void						Inverse();
	DoubleQuaternion			GetInversed() const;

	static DoubleQuaternion		RotationFromTo(const DoubleVec3& from, const DoubleVec3& to);
	static DoubleQuaternion		Lerp(DoubleQuaternion q1, DoubleQuaternion q2, double zeroToOne);
	static DoubleQuaternion		SLerp(DoubleQuaternion q1, DoubleQuaternion q2, double t);

	static DoubleQuaternion		ComputeQuaternion(DoubleVec3 const& v);
	static DoubleVec3			ComputeAngleAxis(DoubleQuaternion const& q);


	bool					operator==(const DoubleQuaternion& compare) const;
	bool					operator!=(const DoubleQuaternion& compare) const;
	const DoubleQuaternion	operator+(const DoubleQuaternion& qToAdd) const;
	const DoubleQuaternion	operator-(const DoubleQuaternion& qToSubtract) const;
	const DoubleQuaternion	operator*(double uniformScale) const;
	const DoubleVec3		operator*(DoubleVec3 vecToMultiply) const;
	const DoubleQuaternion	operator/(double inverseScale) const;
	const DoubleQuaternion	operator*(const DoubleQuaternion& qToMultiply) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const DoubleQuaternion& qToAdd);
	void		operator-=(const DoubleQuaternion& qToSubtract);
	void		operator*=(const DoubleQuaternion& qToMultiply);
	void		operator*=(const double uniformScale);
	void		operator/=(const double uniformDivisor);
	void		operator=(const DoubleQuaternion& copyFrom);

public:
	double i = 0.f;
	double j = 0.f;
	double k = 0.f;
	double w = 1.f;
};

