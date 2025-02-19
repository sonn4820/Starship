#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/MathUtils.hpp"

class CubicHermiteCurve2D;
class CubicHermiteCurve3D;

class CubicBezierCurve2D
{
public :
	CubicBezierCurve2D() = default;
	CubicBezierCurve2D(Vec2 startPos, Vec2 guidePos1, Vec2 guidePos2, Vec2 endPos);
	explicit CubicBezierCurve2D (CubicHermiteCurve2D const& fromHermite);
	Vec2 EvaluateAtParametric(float fromZeroToOne) const;
	float GetApproximateLength(float numSubdivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions = 64) const;

public:
	Vec2 m_startPos;
	Vec2 m_guidePos1;
	Vec2 m_guidePos2;
	Vec2 m_endPos;
};

class CubicHermiteCurve2D
{
public:
	CubicHermiteCurve2D() = default;
	CubicHermiteCurve2D(Vec2 startPos, Vec2 startVel, Vec2 endPos, Vec2 endVel);
	explicit CubicHermiteCurve2D(CubicBezierCurve2D const& fromBezier);
	Vec2 EvaluateAtParametric(float fromZeroToOne) const;
	float GetApproximateLength(float numSubdivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions = 64) const;

public:
	Vec2 m_startPos;
	Vec2 m_endPos;
	Vec2 m_startVel;
	Vec2 m_endVel;
};

class CatmullRomSpline2D
{
public:
	CatmullRomSpline2D() = default;
	CatmullRomSpline2D(std::vector<Vec2> positions);
	void SetPosition(std::vector<Vec2> positions);
	Vec2 EvaluateAtParametric(float t) const;
	Vec2 EvaluateAtParametric_Update(float t);
	float GetApproximateLength(float numSubdivisions = 64) const;
	Vec2 EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions = 64) const;

	std::vector<Vec2> GetPositions() const;
	std::vector<Vec2> GetVelocities() const;

	bool m_hasFinished = false;
private:
	std::vector<Vec2> m_position;
	std::vector<Vec2> m_velocity;
};

class CubicBezierCurve3D
{
public:
	CubicBezierCurve3D() = default;
	CubicBezierCurve3D(Vec3 startPos, Vec3 guidePos1, Vec3 guidePos2, Vec3 endPos);
	explicit CubicBezierCurve3D(CubicHermiteCurve3D const& fromHermite);
	Vec3 EvaluateAtParametric(float fromZeroToOne) const;
	float GetApproximateLength(float numSubdivisions = 64) const;
	Vec3 EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions = 64) const;

public:
	Vec3 m_startPos;
	Vec3 m_guidePos1;
	Vec3 m_guidePos2;
	Vec3 m_endPos;
};

class CubicHermiteCurve3D
{
public:
	CubicHermiteCurve3D() = default;
	CubicHermiteCurve3D(Vec3 startPos, Vec3 startVel, Vec3 endPos, Vec3 endVel);
	explicit CubicHermiteCurve3D(CubicBezierCurve3D const& fromBezier);
	Vec3 EvaluateAtParametric(float fromZeroToOne) const;
	float GetApproximateLength(float numSubdivisions = 64) const;
	Vec3 EvaluateAtApproximateDistance(float distanceAlongCurve, float numSubdivisions = 64) const;

public:
	Vec3 m_startPos;
	Vec3 m_endPos;
	Vec3 m_startVel;
	Vec3 m_endVel;
};