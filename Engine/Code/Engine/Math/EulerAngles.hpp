#pragma once

struct Mat44;
struct Vec3;

struct EulerAngles
{
public:
	EulerAngles() = default;
	EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees);

	void GetAsVectors_IFwd_JLeft_KUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis);
	Mat44 GetAsMatrix_IFwd_JLeft_KUp() const;
	Vec3 GetForwardDir_XFwd_YLeft_ZUp() const;
	void SetFromText(char const* text);
	void TurnToward(EulerAngles goal, float maxDeltaDegrees);

	const EulerAngles	operator*(float uniformScale) const;
	const EulerAngles	operator+(const EulerAngles& EulerAnglesToAdd) const;
	const EulerAngles	operator-(const EulerAngles& EulerAnglesToSubtract) const;
	void				operator+=(const EulerAngles& EulerAnglesToAdd);
	void				operator-=(const EulerAngles& EulerAnglesToSubtract);
	bool				operator<(const EulerAngles & e) const;
	bool				operator<=(const EulerAngles & e) const;
	bool				operator>(const EulerAngles & e) const;
	bool				operator>=(const EulerAngles & e) const;

public:
	float m_yawDegrees = 0.f;
	float m_pitchDegrees = 0.f;
	float m_rollDegrees = 0.f;
};