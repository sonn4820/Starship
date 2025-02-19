#include "Engine/Math/ConvexShape.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Engine/Math/MathUtils.hpp"

ConvexPoly2::ConvexPoly2(std::vector<Vec2> positions)
{
	SetPositions(positions);
}

void ConvexPoly2::SetPositions(std::vector<Vec2> position)
{
	m_vertexPositions = position;
}

std::vector<Vec2> ConvexPoly2::GetPositions() const
{
	return m_vertexPositions;
}

int ConvexPoly2::GetSize() const
{
	return (int)m_vertexPositions.size();
}

void ConvexPoly2::SetPosition(int index, Vec2 newPos)
{
	m_vertexPositions[index] = newPos;
}

void ConvexPoly2::GetBoundingBox(Vec2& out_center, float& out_radius) const
{
	out_center = Vec2();
	out_radius = 0.f;

	for (const auto& vertexPosition : m_vertexPositions)
	{
		out_center += vertexPosition;
	}

	out_center /= (float)GetSize();

	for (const auto& vertexPosition : m_vertexPositions)
	{
		float dist = (vertexPosition - out_center).GetLength();
		if (dist > out_radius)
		{
			out_radius = dist;
		}
	}
}

ConvexPoly2 ConvexPoly2::CreateRandomConvexPoly(Vec2 center, float radius, FloatRange randomDegreeRange, RandomNumberGenerator rng)
{
	std::vector<Vec2> positions;

	float xRandom = rng.RollRandomFloatInRange(-1, 1);
	float yRandom = rng.RollRandomFloatInRange(-1, 1);

	Vec2 initialDir = Vec2(xRandom, yRandom).GetNormalized();

	float startDegree = rng.RollRandomFloatInRange(0.f, 359.f);
	positions.emplace_back(center + Vec2::MakeFromPolarDegrees(startDegree, radius));

	float currenTheta = startDegree;

	while (true)
	{
		float moveDegree = rng.RollRandomFloatInRange(randomDegreeRange.m_min, randomDegreeRange.m_max);
		currenTheta += moveDegree;

		if (currenTheta > startDegree + 360 - randomDegreeRange.m_min)
		{
			break;
		}

		positions.emplace_back(center + Vec2::MakeFromPolarDegrees(currenTheta, radius));
	}

	ConvexPoly2 newConvex;
	newConvex.SetPositions(positions);

	return newConvex;

}

ConvexHull2::ConvexHull2(std::vector<Plane2> planes)
{
	SetPlanes(planes);
}

ConvexHull2::ConvexHull2(ConvexPoly2 poly)
{
	for (size_t i = 0; i < poly.GetSize(); i++)
	{
		Vec2 dir;
		if (i == poly.GetSize() - 1)
		{
			dir = poly.GetPositions()[0] - poly.GetPositions()[i];
		}
		else
		{
			dir = poly.GetPositions()[i + 1] - poly.GetPositions()[i];
		}

		Vec2 planeNormal = dir.GetRotatedMinus90Degrees().GetNormalized();
		float distance = DotProduct2D(planeNormal, poly.GetPositions()[i]);

		m_boundingPlanes.emplace_back(planeNormal, distance);
	}
}

void ConvexHull2::SetPlanes(std::vector<Plane2> planes)
{
	m_boundingPlanes = planes;
}

std::vector<Plane2> ConvexHull2::GetPlanes() const
{
	return m_boundingPlanes;
}

int ConvexHull2::GetSize() const
{
	return (int)m_boundingPlanes.size();
}

bool ConvexHull2::IsPointInside(Vec2 referencePoint)
{
	for (const auto & boundingPlane : m_boundingPlanes)
	{
		if (boundingPlane.GetAltitudeOfPoint(referencePoint) >= 0)
		{
			return false;
		}
	}

	return true;
}
