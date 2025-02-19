#include "Engine/Math/Spline.hpp"

CubicBezierCurve2D::CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos)
	:m_startPos(startPos), m_guidePos1(guidePos1), m_guidePos2(guidePos2), m_endPos(endPos)
{
}

CubicBezierCurve2D::CubicBezierCurve2D(CubicHermiteCurve2D const& fromHermite)
{
	m_startPos = fromHermite.m_startPos;
	m_endPos = fromHermite.m_endPos;
	m_guidePos1 = m_startPos + (1.f / 3.f) * fromHermite.m_startVel;
	m_guidePos2 = m_endPos - (1.f / 3.f) * fromHermite.m_endVel;
}

Vec2 CubicBezierCurve2D::EvaluateAtParametric(float fromZeroToOne) const
{
	float x = ComputeCubicBezier1D(fromZeroToOne, m_startPos.x, m_guidePos1.x, m_guidePos2.x, m_endPos.x);
	float y = ComputeCubicBezier1D(fromZeroToOne, m_startPos.y, m_guidePos1.y, m_guidePos2.y, m_endPos.y);
	return Vec2(x, y);
}

float CubicBezierCurve2D::GetApproximateLength(float numSubdivisions /*= 64*/) const
{
	float length = 0.f;
	Vec2 prevPos = m_startPos;

	for (float t = 0.f; t <= 1.f; t += 1.f/ numSubdivisions)
	{
		Vec2 currentPoint = EvaluateAtParametric(t);
		length += (currentPoint - prevPos).GetLength();
		prevPos = currentPoint;
	}

	return length;
}

Vec2 CubicBezierCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions /*= 64*/) const
{
	float approxLength = GetApproximateLength(numSubdivisions);

	if (distanceAlongCurve <= 0.0f || approxLength <= 0.0f) 
	{
		return m_startPos;
	}
	else if (distanceAlongCurve >= approxLength) 
	{
		return m_endPos;
	}

	float targetDistance = 0.0f;
	Vec2 prevPos = m_startPos;

	for (float currentT = 0.f; currentT <= 1.f; currentT += 1.f / numSubdivisions)
	{
		Vec2 currentPoint = EvaluateAtParametric(currentT);
		float segmentLength = (currentPoint - prevPos).GetLength();

		if (targetDistance + segmentLength >= distanceAlongCurve)
		{
			return Interpolate(prevPos, currentPoint, (distanceAlongCurve - targetDistance) / segmentLength) ;
		}
		targetDistance += segmentLength;
		prevPos = currentPoint;
	}

	return m_endPos;
}

CubicHermiteCurve2D::CubicHermiteCurve2D(Vec2 startPos, Vec2 startVel, Vec2 endPos, Vec2 endVel)
	:m_startPos(startPos), m_startVel(startVel), m_endPos(endPos), m_endVel(endVel)
{
}

CubicHermiteCurve2D::CubicHermiteCurve2D(CubicBezierCurve2D const& fromBezier)
{
	m_startPos = fromBezier.m_startPos;
	m_endPos = fromBezier.m_endPos;
	m_startVel = 3.f * (fromBezier.m_guidePos1 - fromBezier.m_startPos);
	m_endVel = 3.f * (fromBezier.m_endPos - fromBezier.m_guidePos2);
}

Vec2 CubicHermiteCurve2D::EvaluateAtParametric(float fromZeroToOne) const
{
	Vec2 guidePos1 = m_startPos + (1.f / 3.f) * m_startVel;
	Vec2 guidePos2 = m_endPos - (1.f / 3.f) * m_endVel;
	float x = ComputeCubicBezier1D(fromZeroToOne, m_startPos.x, guidePos1.x, guidePos2.x, m_endPos.x);
	float y = ComputeCubicBezier1D(fromZeroToOne, m_startPos.y, guidePos1.y, guidePos2.y, m_endPos.y);
	return Vec2(x, y);
}

float CubicHermiteCurve2D::GetApproximateLength(float numSubdivisions /*= 64*/) const
{
	float length = 0.f;
	Vec2 prevPos = m_startPos;

	for (float t = 0.f; t <= 1.f; t += 1.f / numSubdivisions)
	{
		Vec2 currentPoint = EvaluateAtParametric(t);
		length += (currentPoint - prevPos).GetLength();
		prevPos = currentPoint;
	}

	return length;
}

Vec2 CubicHermiteCurve2D::EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions) const
{
	float approxLength = GetApproximateLength(numSubdivisions);

	if (distanceAlongCurve <= 0.0f || approxLength <= 0.0f)
	{
		return m_startPos;
	}
	else if (distanceAlongCurve >= approxLength)
	{
		return m_endPos;
	}

	float targetDistance = 0.0f;
	Vec2 prevPos = m_startPos;

	for (float currentT = 0.f; currentT <= 1.f; currentT += 1.f / numSubdivisions)
	{
		Vec2 currentPoint = EvaluateAtParametric(currentT);
		float segmentLength = (currentPoint - prevPos).GetLength();

		if (targetDistance + segmentLength >= distanceAlongCurve)
		{
			return Interpolate(prevPos, currentPoint, (distanceAlongCurve - targetDistance) / segmentLength);
		}
		targetDistance += segmentLength;
		prevPos = currentPoint;
	}

	return m_endPos;
}

CatmullRomSpline2D::CatmullRomSpline2D(std::vector<Vec2> positions)
{
	SetPosition(positions);
}

void CatmullRomSpline2D::SetPosition(std::vector<Vec2> positions)
{
	m_position.clear();
	m_velocity.clear();
	m_position = positions;
	m_velocity.resize(m_position.size());

	m_velocity[0] = Vec2();
	m_velocity[m_velocity.size() - 1] = Vec2();

	for (size_t i = 1; i < m_velocity.size() - 1; i++)
	{
		Vec2 prevPos = m_position[i - 1];
		Vec2 nextPos = m_position[i + 1];

		m_velocity[i] = (nextPos - prevPos) / 2.f;
	}
}

Vec2 CatmullRomSpline2D::EvaluateAtParametric(float t) const
{
	int currentCurve = RoundDownToInt(t);
	float currentTOnThatCurve = t - (float)currentCurve;
	CubicHermiteCurve2D curve;
	Vec2 evaluatedVavlue;
	if (currentCurve < (int)m_position.size() - 1)
	{
		curve = CubicHermiteCurve2D(m_position[currentCurve], m_velocity[currentCurve], m_position[currentCurve + 1], m_velocity[currentCurve + 1]);
		evaluatedVavlue = curve.EvaluateAtParametric(currentTOnThatCurve);
	}
	else
	{
		evaluatedVavlue = m_position[m_position.size() - 1];
	}

	return evaluatedVavlue;
}

Vec2 CatmullRomSpline2D::EvaluateAtParametric_Update(float t)
{
	int currentCurve = RoundDownToInt(t);
	float currentTOnThatCurve = t - (float)currentCurve;
	CubicHermiteCurve2D curve;
	Vec2 evaluatedVavlue;
	if (currentCurve < (int)m_position.size() - 1)
	{
		curve = CubicHermiteCurve2D(m_position[currentCurve], m_velocity[currentCurve], m_position[currentCurve + 1], m_velocity[currentCurve + 1]);
		evaluatedVavlue = curve.EvaluateAtParametric(currentTOnThatCurve);
	}
	else
	{
		m_hasFinished = true;
		evaluatedVavlue = m_position[m_position.size() - 1];
	}

	return evaluatedVavlue;
}

float CatmullRomSpline2D::GetApproximateLength(float numSubdivisions) const
{
	float length = 0.f;
	Vec2 prevPos = m_position[0];

	for (float t = 0.f; t <= (float)(m_position.size() - 1); t += (float)(m_position.size() - 1) / numSubdivisions)
	{
		Vec2 currentPoint = EvaluateAtParametric(t);
		length += (currentPoint - prevPos).GetLength();
		prevPos = currentPoint;
	}

	return length;
}

Vec2 CatmullRomSpline2D::EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions) const
{
	float approxLength = GetApproximateLength(numSubdivisions);

	if (distanceAlongCurve <= 0.0f || approxLength <= 0.0f)
	{
		return m_position[0];
	}
	else if (distanceAlongCurve >= approxLength)
	{
		return m_position[m_position.size() - 1];
	}

	float targetDistance = 0.0f;
	Vec2 prevPos = m_position[0];

	for (float currentT = 0.f; currentT <= (float)(m_position.size() - 1); currentT += (float)(m_position.size() - 1) / numSubdivisions)
	{
		Vec2 currentPoint = EvaluateAtParametric(currentT);
		float segmentLength = (currentPoint - prevPos).GetLength();

		if (targetDistance + segmentLength >= distanceAlongCurve)
		{
			return Interpolate(prevPos, currentPoint, (distanceAlongCurve - targetDistance) / segmentLength);
		}
		targetDistance += segmentLength;
		prevPos = currentPoint;
	}

	return m_position[m_position.size() - 1];
}

std::vector<Vec2> CatmullRomSpline2D::GetPositions() const
{
	return m_position;
}

std::vector<Vec2> CatmullRomSpline2D::GetVelocities() const
{
	return m_velocity;
}

CubicBezierCurve3D::CubicBezierCurve3D(Vec3 startPos, Vec3 guidePos1, Vec3 guidePos2, Vec3 endPos)
	:m_startPos(startPos), m_guidePos1(guidePos1), m_guidePos2(guidePos2), m_endPos(endPos)
{

}

CubicBezierCurve3D::CubicBezierCurve3D(CubicHermiteCurve3D const& fromHermite)
{
	m_startPos = fromHermite.m_startPos;
	m_endPos = fromHermite.m_endPos;
	m_guidePos1 = m_startPos + (1.f / 3.f) * fromHermite.m_startVel;
	m_guidePos2 = m_endPos - (1.f / 3.f) * fromHermite.m_endVel;
}

Vec3 CubicBezierCurve3D::EvaluateAtParametric(float fromZeroToOne) const
{
	float x = ComputeCubicBezier1D(fromZeroToOne, m_startPos.x, m_guidePos1.x, m_guidePos2.x, m_endPos.x);
	float y = ComputeCubicBezier1D(fromZeroToOne, m_startPos.y, m_guidePos1.y, m_guidePos2.y, m_endPos.y);
	float z = ComputeCubicBezier1D(fromZeroToOne, m_startPos.z, m_guidePos1.z, m_guidePos2.z, m_endPos.z);
	return Vec3(x, y, z);
}

float CubicBezierCurve3D::GetApproximateLength(float numSubdivisions /*= 64*/) const
{
	float length = 0.f;
	Vec3 prevPos = m_startPos;

	for (float t = 0.f; t <= 1.f; t += 1.f / numSubdivisions)
	{
		Vec3 currentPoint = EvaluateAtParametric(t);
		length += (currentPoint - prevPos).GetLength();
		prevPos = currentPoint;
	}

	return length;
}

Vec3 CubicBezierCurve3D::EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions /*= 64*/) const
{
	float approxLength = GetApproximateLength(numSubdivisions);

	if (distanceAlongCurve <= 0.0f || approxLength <= 0.0f)
	{
		return m_startPos;
	}
	else if (distanceAlongCurve >= approxLength)
	{
		return m_endPos;
	}

	float targetDistance = 0.0f;
	Vec3 prevPos = m_startPos;

	for (float currentT = 0.f; currentT <= 1.f; currentT += 1.f / numSubdivisions)
	{
		Vec3 currentPoint = EvaluateAtParametric(currentT);
		float segmentLength = (currentPoint - prevPos).GetLength();

		if (targetDistance + segmentLength >= distanceAlongCurve)
		{
			return Interpolate(prevPos, currentPoint, (distanceAlongCurve - targetDistance) / segmentLength);
		}
		targetDistance += segmentLength;
		prevPos = currentPoint;
	}

	return m_endPos;
}

CubicHermiteCurve3D::CubicHermiteCurve3D(Vec3 startPos, Vec3 startVel, Vec3 endPos, Vec3 endVel)
	:m_startPos(startPos), m_startVel(startVel), m_endPos(endPos), m_endVel(endVel)
{

}

CubicHermiteCurve3D::CubicHermiteCurve3D(CubicBezierCurve3D const& fromBezier)
{
	m_startPos = fromBezier.m_startPos;
	m_endPos = fromBezier.m_endPos;
	m_startVel = 3.f * (fromBezier.m_guidePos1 - fromBezier.m_startPos);
	m_endVel = 3.f * (fromBezier.m_endPos - fromBezier.m_guidePos2);
}

Vec3 CubicHermiteCurve3D::EvaluateAtParametric(float fromZeroToOne) const
{
	Vec3 guidePos1 = m_startPos + (1.f / 3.f) * m_startVel;
	Vec3 guidePos2 = m_endPos - (1.f / 3.f) * m_endVel;
	float x = ComputeCubicBezier1D(fromZeroToOne, m_startPos.x, guidePos1.x, guidePos2.x, m_endPos.x);
	float y = ComputeCubicBezier1D(fromZeroToOne, m_startPos.y, guidePos1.y, guidePos2.y, m_endPos.y);
	float z = ComputeCubicBezier1D(fromZeroToOne, m_startPos.z, guidePos1.z, guidePos2.z, m_endPos.z);
	return Vec3(x, y, z);
}

float CubicHermiteCurve3D::GetApproximateLength(float numSubdivisions /*= 64*/) const
{
	float length = 0.f;
	Vec3 prevPos = m_startPos;

	for (float t = 0.f; t <= 1.f; t += 1.f / numSubdivisions)
	{
		Vec3 currentPoint = EvaluateAtParametric(t);
		length += (currentPoint - prevPos).GetLength();
		prevPos = currentPoint;
	}

	return length;
}

Vec3 CubicHermiteCurve3D::EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions /*= 64*/) const
{
	float approxLength = GetApproximateLength(numSubdivisions);

	if (distanceAlongCurve <= 0.0f || approxLength <= 0.0f)
	{
		return m_startPos;
	}
	else if (distanceAlongCurve >= approxLength)
	{
		return m_endPos;
	}

	float targetDistance = 0.0f;
	Vec3 prevPos = m_startPos;

	for (float currentT = 0.f; currentT <= 1.f; currentT += 1.f / numSubdivisions)
	{
		Vec3 currentPoint = EvaluateAtParametric(currentT);
		float segmentLength = (currentPoint - prevPos).GetLength();

		if (targetDistance + segmentLength >= distanceAlongCurve)
		{
			return Interpolate(prevPos, currentPoint, (distanceAlongCurve - targetDistance) / segmentLength);
		}
		targetDistance += segmentLength;
		prevPos = currentPoint;
	}

	return m_endPos;
}
