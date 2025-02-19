#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Mat44.hpp"

EulerAngles::EulerAngles(float yawDegrees, float pitchDegrees, float rollDegrees)
	:m_yawDegrees(yawDegrees), m_pitchDegrees(pitchDegrees), m_rollDegrees(rollDegrees)
{
}

void EulerAngles::GetAsVectors_IFwd_JLeft_KUp(Vec3& out_forwardIBasis, Vec3& out_leftJBasis, Vec3& out_upKBasis)
{
	float Cy = CosDegrees(m_yawDegrees);
	float Sy = SinDegrees(m_yawDegrees);
	float Cp = CosDegrees(m_pitchDegrees);
	float Sp = SinDegrees(m_pitchDegrees);
	float Cr = CosDegrees(m_rollDegrees);
	float Sr = SinDegrees(m_rollDegrees);

	out_forwardIBasis = Vec3(Cy * Cp, Cp * Sy, -Sp);
	out_leftJBasis = Vec3(Sr * Sp * Cy - Sy * Cr, Sr * Sp * Sy + Cr * Cy, Sr * Cp);
	out_upKBasis = Vec3(Sp * Cr * Cy + Sy * Sr, Sp * Cr * Sy - Sr * Cy, Cp * Cr);
}

Mat44 EulerAngles::GetAsMatrix_IFwd_JLeft_KUp() const
{
	Mat44 result;
	float Cy = CosDegrees(m_yawDegrees);
	float Sy = SinDegrees(m_yawDegrees);
	float Cp = CosDegrees(m_pitchDegrees);
	float Sp = SinDegrees(m_pitchDegrees);
	float Cr = CosDegrees(m_rollDegrees);
	float Sr = SinDegrees(m_rollDegrees);

	result.m_values[result.Ix] = Cy * Cp;
	result.m_values[result.Iy] = Cp * Sy;
	result.m_values[result.Iz] = -Sp;

	result.m_values[result.Jx] = Sr * Sp * Cy - Sy * Cr;
	result.m_values[result.Jy] = Sr * Sp * Sy + Cr * Cy;
	result.m_values[result.Jz] = Sr * Cp;

	result.m_values[result.Kx] = Sp * Cr * Cy + Sy * Sr;
	result.m_values[result.Ky] = Sp * Cr * Sy - Sr * Cy;
	result.m_values[result.Kz] = Cp * Cr;

	return result;
}

Vec3 EulerAngles::GetForwardDir_XFwd_YLeft_ZUp() const
{
	float cy = CosDegrees(m_yawDegrees);
	float sy = SinDegrees(m_yawDegrees);
	float cp = CosDegrees(m_pitchDegrees);
	float sp = SinDegrees(m_pitchDegrees);

	return Vec3(cy * cp, sy * cp, -sp);
}

const EulerAngles EulerAngles::operator*(float uniformScale) const
{
	return EulerAngles(m_yawDegrees * uniformScale, m_pitchDegrees * uniformScale, m_rollDegrees * uniformScale);
}

const EulerAngles EulerAngles::operator+(const EulerAngles& EulerAnglesToAdd) const
{
	return EulerAngles(m_yawDegrees + EulerAnglesToAdd.m_yawDegrees, m_pitchDegrees + EulerAnglesToAdd.m_pitchDegrees, m_rollDegrees + EulerAnglesToAdd.m_rollDegrees);
}
const EulerAngles EulerAngles::operator-(const EulerAngles& EulerAnglesToSubtract) const
{
	return EulerAngles(m_yawDegrees - EulerAnglesToSubtract.m_yawDegrees, m_pitchDegrees - EulerAnglesToSubtract.m_pitchDegrees, m_rollDegrees - EulerAnglesToSubtract.m_rollDegrees);
}

bool EulerAngles::operator<(const EulerAngles& e) const
{
	return (double)m_yawDegrees < (double)e.m_yawDegrees 
		&& (double)m_pitchDegrees < (double)e.m_pitchDegrees 
		&& (double)m_rollDegrees < (double)e.m_rollDegrees;
}

bool EulerAngles::operator<=(const EulerAngles& e) const
{
	return (double)m_yawDegrees <= (double)e.m_yawDegrees
		&& (double)m_pitchDegrees <= (double)e.m_pitchDegrees
		&& (double)m_rollDegrees <= (double)e.m_rollDegrees;
}

bool EulerAngles::operator>(const EulerAngles& e) const
{
	return (double)m_yawDegrees > (double)e.m_yawDegrees
		&& (double)m_pitchDegrees > (double)e.m_pitchDegrees
		&& (double)m_rollDegrees > (double)e.m_rollDegrees;
}

bool EulerAngles::operator>=(const EulerAngles& e) const
{
	return (double)m_yawDegrees >= (double)e.m_yawDegrees
		&& (double)m_pitchDegrees >= (double)e.m_pitchDegrees
		&& (double)m_rollDegrees >= (double)e.m_rollDegrees;
}

void EulerAngles::operator+=(const EulerAngles& EulerAnglesToAdd)
{
	m_yawDegrees += EulerAnglesToAdd.m_yawDegrees;
	m_pitchDegrees += EulerAnglesToAdd.m_pitchDegrees;
	m_rollDegrees += EulerAnglesToAdd.m_rollDegrees;
}

void EulerAngles::operator-=(const EulerAngles& EulerAnglesToSubtract)
{
	m_yawDegrees -= EulerAnglesToSubtract.m_yawDegrees;
	m_pitchDegrees -= EulerAnglesToSubtract.m_pitchDegrees;
	m_rollDegrees -= EulerAnglesToSubtract.m_rollDegrees;
}



void EulerAngles::SetFromText(char const* text)
{
	Strings strings = SplitStringOnDelimiter(text, ',');

	if (strings.size() != 3)
	{
		ERROR_AND_DIE("INPUT WRONG FORMAT VEC3");
	}
	const char* yawChar = strings[0].c_str();
	const char* pitchChar = strings[1].c_str();
	const char* rollChar = strings[2].c_str();

	m_yawDegrees = static_cast<float>(atof(yawChar));
	m_pitchDegrees = static_cast<float>(atof(pitchChar));
	m_rollDegrees = static_cast<float>(atof(rollChar));
}

void EulerAngles::TurnToward(EulerAngles goal, float maxDeltaDegrees)
{
	m_yawDegrees = GetTurnedTowardDegrees(m_yawDegrees, goal.m_yawDegrees, maxDeltaDegrees);
	m_pitchDegrees = GetTurnedTowardDegrees(m_pitchDegrees, goal.m_pitchDegrees, maxDeltaDegrees);
	m_rollDegrees = GetTurnedTowardDegrees(m_rollDegrees, goal.m_rollDegrees, maxDeltaDegrees);
}
