#include "Engine/Math/DoubleAABB3.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

DoubleAABB3::DoubleAABB3(DoubleAABB3 const& copyfrom)
	: m_mins(copyfrom.m_mins), m_maxs(copyfrom.m_maxs)
{
}

DoubleAABB3::DoubleAABB3(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
	:m_mins(DoubleVec3(minX, minY, minZ)), m_maxs(DoubleVec3(maxX, maxY, maxZ))
{
}

DoubleAABB3::DoubleAABB3(DoubleVec3 const& mins, DoubleVec3 const& maxs)
	:m_mins(mins), m_maxs(maxs)
{
}

DoubleAABB3::DoubleAABB3(DoubleVec3 center, double height, double widthX, double widthY)
{
	m_mins = DoubleVec3(center.x - widthX, center.y - widthY, center.z - height);
	m_maxs = DoubleVec3(center.x + widthX, center.y + widthY, center.z + height);
}

DoubleAABB3::DoubleAABB3(AABB3 const& aabb)
	: m_mins(aabb.m_mins), m_maxs(aabb.m_maxs)
{

}

void DoubleAABB3::StretchToIncludePoint(DoubleVec3 const& point)
{
	if (point.x > m_maxs.x) m_maxs = DoubleVec3(point.x, m_maxs.y, m_maxs.z);
	if (point.x < m_mins.x)	m_mins = DoubleVec3(point.x, m_mins.y, m_mins.z);
	if (point.y > m_maxs.y)	m_maxs = DoubleVec3(m_maxs.x, point.y, m_maxs.z);
	if (point.y < m_mins.y)	m_mins = DoubleVec3(m_mins.x, point.y, m_mins.z);
	if (point.z > m_maxs.z)	m_maxs = DoubleVec3(m_maxs.x, m_maxs.y, point.z);
	if (point.z < m_mins.z)	m_mins = DoubleVec3(m_mins.x, m_mins.y, point.z);

}

void DoubleAABB3::GetCornerPoints(DoubleVec3* out_fourCornerWorldPositions) const
{
	out_fourCornerWorldPositions[0] = DoubleVec3(m_mins.x, m_mins.y, m_mins.z);
	out_fourCornerWorldPositions[1] = DoubleVec3(m_maxs.x, m_mins.y, m_mins.z);
	out_fourCornerWorldPositions[2] = DoubleVec3(m_mins.x, m_maxs.y, m_mins.z);
	out_fourCornerWorldPositions[3] = DoubleVec3(m_maxs.x, m_maxs.y, m_mins.z);

	out_fourCornerWorldPositions[4] = DoubleVec3(m_mins.x, m_mins.y, m_maxs.z);
	out_fourCornerWorldPositions[5] = DoubleVec3(m_maxs.x, m_mins.y, m_maxs.z);
	out_fourCornerWorldPositions[6] = DoubleVec3(m_mins.x, m_maxs.y, m_maxs.z);
	out_fourCornerWorldPositions[7] = DoubleVec3(m_maxs.x, m_maxs.y, m_maxs.z);
}

DoubleVec3 DoubleAABB3::GetCenter() const
{
	return DoubleVec3((m_mins.x + m_maxs.x) * 0.5f, (m_mins.y + m_maxs.y) * 0.5f, (m_mins.z + m_maxs.z) * 0.5f);
}

DoubleVec2 DoubleAABB3::GetCenterXY() const
{
	return DoubleVec2((m_mins.x + m_maxs.x) * 0.5f, (m_mins.y + m_maxs.y) * 0.5f);
}

DoubleVec3 DoubleAABB3::GetNearestEdgePosition(DoubleVec3 const& point) const
{
	DoubleVec3 edgePos = point;
	DoubleVec3 center = GetCenter();
	DoubleVec3 halfDimensions = GetHalfDimension();

	double distToEdges[6];
	distToEdges[0] = ((center.x + halfDimensions.x) - point.x);
	distToEdges[1] = (point.x - (center.x - halfDimensions.x));
	distToEdges[2] = ((center.y + halfDimensions.y) - point.y);
	distToEdges[3] = (point.y - (center.y - halfDimensions.y));
	distToEdges[4] = ((center.z + halfDimensions.z) - point.z);
	distToEdges[5] = (point.z - (center.z - halfDimensions.z));

	double bestDist = FLT_MAX;
	int bestDistIndex = -1;
	double secondBestDist = FLT_MAX;
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

DoubleVec3 DoubleAABB3::GetDimension() const
{
	return DoubleVec3(m_maxs.x - m_mins.x, m_maxs.y - m_mins.y, m_maxs.z - m_mins.z);
}

DoubleVec3 DoubleAABB3::GetHalfDimension() const
{
	return GetDimension() * 0.5f;
}

bool DoubleAABB3::IsPointInside(DoubleVec3 const& point) const
{
	return (point.x <= m_maxs.x && point.x >= m_mins.x)
		&& (point.y <= m_maxs.y && point.y >= m_mins.y)
		&& (point.z <= m_maxs.z && point.z >= m_mins.z);
}

DoubleVec3 const DoubleAABB3::GetNearestPoint(DoubleVec3 const& referencePosition) const
{
	DoubleVec3 clampedPosition = referencePosition;
	clampedPosition.x = Clamp_Double(clampedPosition.x, m_mins.x, m_maxs.x);
	clampedPosition.y = Clamp_Double(clampedPosition.y, m_mins.y, m_maxs.y);
	clampedPosition.z = Clamp_Double(clampedPosition.z, m_mins.z, m_maxs.z);
	return clampedPosition;
}

DoubleAABB3 DoubleAABB3::ZERO_TO_ONE = DoubleAABB3(0, 0, 0, 1, 1, 1);