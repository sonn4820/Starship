#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

AABB3::AABB3(AABB3 const& copyfrom)
	: m_mins(copyfrom.m_mins), m_maxs(copyfrom.m_maxs)
{
}

AABB3::AABB3(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	:m_mins(Vec3(minX, minY, minZ)), m_maxs(Vec3(maxX, maxY, maxZ))
{
}

AABB3::AABB3(Vec3 const& mins, Vec3 const& maxs)
	:m_mins(mins), m_maxs(maxs)
{
}

AABB3::AABB3(Vec3 center, float height, float widthX, float widthY)
{
	m_mins = Vec3(center.x - widthX, center.y - widthY, center.z - height);
	m_maxs = Vec3(center.x + widthX, center.y + widthY, center.z + height);
}

AABB3::AABB3(DoubleAABB3 const& doubleAABB3)
	: m_mins(doubleAABB3.m_mins), m_maxs(doubleAABB3.m_maxs)
{

}

void AABB3::StretchToIncludePoint(Vec3 const& point)
{
	if (point.x > m_maxs.x) m_maxs = Vec3(point.x, m_maxs.y, m_maxs.z);
	if (point.x < m_mins.x)	m_mins = Vec3(point.x, m_mins.y, m_mins.z);
	if (point.y > m_maxs.y)	m_maxs = Vec3(m_maxs.x, point.y, m_maxs.z);
	if (point.y < m_mins.y)	m_mins = Vec3(m_mins.x, point.y, m_mins.z);
	if (point.z > m_maxs.z)	m_maxs = Vec3(m_maxs.x, m_maxs.y, point.z);
	if (point.z < m_mins.z)	m_mins = Vec3(m_mins.x, m_mins.y, point.z);

}

void AABB3::GetCornerPoints(Vec3* out_fourCornerWorldPositions) const
{
	out_fourCornerWorldPositions[0] = Vec3(m_mins.x, m_mins.y, m_mins.z);
	out_fourCornerWorldPositions[1] = Vec3(m_maxs.x, m_mins.y, m_mins.z);
	out_fourCornerWorldPositions[2] = Vec3(m_mins.x, m_maxs.y, m_mins.z);
	out_fourCornerWorldPositions[3] = Vec3(m_maxs.x, m_maxs.y, m_mins.z);

	out_fourCornerWorldPositions[4] = Vec3(m_mins.x, m_mins.y, m_maxs.z);
	out_fourCornerWorldPositions[5] = Vec3(m_maxs.x, m_mins.y, m_maxs.z);
	out_fourCornerWorldPositions[6] = Vec3(m_mins.x, m_maxs.y, m_maxs.z);
	out_fourCornerWorldPositions[7] = Vec3(m_maxs.x, m_maxs.y, m_maxs.z);
}

Vec3 AABB3::GetCenter() const
{
	return Vec3((m_mins.x + m_maxs.x) * 0.5f, (m_mins.y + m_maxs.y) * 0.5f, (m_mins.z + m_maxs.z) * 0.5f);
}

Vec2 AABB3::GetCenterXY() const
{
	return Vec2((m_mins.x + m_maxs.x) * 0.5f, (m_mins.y + m_maxs.y) * 0.5f);
}

Vec3 AABB3::GetNearestEdgePosition(Vec3 const& point) const
{
	Vec3 edgePos = point;
	Vec3 center = GetCenter();
	Vec3 halfDimensions = GetHalfDimension();

	float distToEdges[6];
	distToEdges[0] = ((center.x + halfDimensions.x) - point.x);
	distToEdges[1] = (point.x - (center.x - halfDimensions.x));
	distToEdges[2] = ((center.y + halfDimensions.y) - point.y);
	distToEdges[3] = (point.y - (center.y - halfDimensions.y));
	distToEdges[4] = ((center.z + halfDimensions.z) - point.z);
	distToEdges[5] = (point.z - (center.z - halfDimensions.z));

	float bestDist = FLT_MAX;
	int bestDistIndex = -1;
	float secondBestDist = FLT_MAX;
	int secondBestDistIndex = -1;

	for (int i = 0; i < 6; i++)
	{
		if (distToEdges[i] < bestDist)
		{
			secondBestDist = bestDist;
			secondBestDistIndex = bestDistIndex;
			bestDist = distToEdges[i];
			bestDistIndex = i;
		}
		else if (distToEdges[i] < secondBestDist)
		{
			secondBestDist = distToEdges[i];
			secondBestDistIndex = i;
		}
	}

	//Side 1
	switch (bestDistIndex)
	{
	case 0:
		edgePos.x += distToEdges[bestDistIndex];
		break;
	case 1:
		edgePos.x -= distToEdges[bestDistIndex];
		break;
	case 2:
		edgePos.y += distToEdges[bestDistIndex];
		break;
	case 3:
		edgePos.y -= distToEdges[bestDistIndex];
		break;
	case 4:
		edgePos.z += distToEdges[bestDistIndex];
		break;
	case 5:
		edgePos.z -= distToEdges[bestDistIndex];
		break;
	}

	//Side 2
	switch (secondBestDistIndex)
	{
	case 0:
		edgePos.x += distToEdges[secondBestDistIndex];
		break;
	case 1:
		edgePos.x -= distToEdges[secondBestDistIndex];
		break;
	case 2:
		edgePos.y += distToEdges[secondBestDistIndex];
		break;
	case 3:
		edgePos.y -= distToEdges[secondBestDistIndex];
		break;
	case 4:
		edgePos.z += distToEdges[secondBestDistIndex];
		break;
	case 5:
		edgePos.z -= distToEdges[secondBestDistIndex];
		break;
	}

	return edgePos;
}

Vec3 AABB3::GetDimension() const
{
	return Vec3(m_maxs.x - m_mins.x, m_maxs.y - m_mins.y, m_maxs.z - m_mins.z);
}

Vec3 AABB3::GetHalfDimension() const
{
	return GetDimension() * 0.5f;
}

bool AABB3::IsPointInside(Vec3 const& point) const
{
	return (point.x <= m_maxs.x && point.x >= m_mins.x)
		&& (point.y <= m_maxs.y && point.y >= m_mins.y)
		&& (point.z <= m_maxs.z && point.z >= m_mins.z);
}

Vec3 const AABB3::GetNearestPoint(Vec3 const& referencePosition) const
{
	Vec3 clampedPosition = referencePosition;
	clampedPosition.x = Clamp(clampedPosition.x, m_mins.x, m_maxs.x);
	clampedPosition.y = Clamp(clampedPosition.y, m_mins.y, m_maxs.y);
	clampedPosition.z = Clamp(clampedPosition.z, m_mins.z, m_maxs.z);
	return clampedPosition;
}

Vec3 AABB3::support(const Vec3& direction) const
{
	UNUSED(direction);
	return Vec3();
}

AABB3 AABB3::ZERO_TO_ONE = AABB3(0.f, 0.f, 0.f, 1.f, 1.f, 1.f);