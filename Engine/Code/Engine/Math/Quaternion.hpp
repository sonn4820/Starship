#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Math/EulerAngles.hpp"

//SOURCE: https://irrlicht.sourceforge.io/docu/quaternion_8h_source.html?fbclid=IwZXh0bgNhZW0CMTAAAR0kwVfx5jXQVhbodQtewKJ7pS6S5nOY70o7VwNy8G6oZdYyk02OOgOIe2Q_aem_ARWr5VLrwj54SUNrwqcZvP9zoPe_SEQuLk74XPXsfeWV7bLR0cXeqPPBzLlVjOuCQvRbiLtGhMTOgLboO3NNDQxf

struct DoubleQuaternion;
struct Quaternion
{
public:
	Quaternion() = default;
	Quaternion(DoubleQuaternion q);
	Quaternion(Quaternion const& copyFrom);
	Quaternion(float i, float j, float k);
	Quaternion(float i, float j, float k, float w);
	Quaternion(EulerAngles const& eulerAngle);
	Quaternion(Mat44 const& mat);
	explicit Quaternion(Vec3 const& rotationVec);
	Quaternion(float angle, Vec3 const& axis);

	Mat44		GetMatrix(Vec3 translation = Vec3()) const;

	Quaternion	GetNormalized() const;
	void		Normalize();
	EulerAngles	ToEuler();
	void		ToAngleAxis(float& angleRadian, Vec3& axis) const;
	static Quaternion RotationFromTo(const Vec3& from, const Vec3& to);

	static Quaternion Lerp(Quaternion q1, Quaternion q2, float zeroToOne);
	static Quaternion SLerp(Quaternion q1, Quaternion q2, float deltaTime, float threshold = 0.5f);

	bool		Equal(Quaternion const& q, float tolerance = 0.005f);
	float		DotProduct(Quaternion& q);
	float		GetAngle();
	Vec3		Rotate(const Vec3& v);

	float		GetMagnitude() const;
	void		Conjugate();
	Quaternion	GetConjugated() const;
	void		Inverse();
	Quaternion	GetInversed() const;

	bool				operator==(const Quaternion& compare) const;
	bool				operator!=(const Quaternion& compare) const;
	const Quaternion	operator+(const Quaternion& qToAdd) const;
	const Quaternion	operator-(const Quaternion& qToSubtract) const;
	const Quaternion	operator*(float uniformScale) const;
	const Vec3			operator*(Vec3 vecToMultiply) const;
	const Quaternion	operator/(float inverseScale) const;
	const Quaternion	operator*(const Quaternion& qToMultiply) const;

	// Operators (self-mutating / non-const)
	void		operator+=(const Quaternion& qToAdd);
	void		operator-=(const Quaternion& qToSubtract);
	void		operator*=(const Quaternion& qToMultiply);
	void		operator*=(const float uniformScale);
	void		operator/=(const float uniformDivisor);
	void		operator=(const Quaternion& copyFrom);

public:
	float i = 0.f;
	float j = 0.f;
	float k = 0.f;
	float w = 1.f;
};

