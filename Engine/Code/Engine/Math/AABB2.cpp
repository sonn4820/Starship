#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/MathUtils.hpp"

AABB2::AABB2(AABB2 const& copyfrom)
{
	m_mins = copyfrom.m_mins;
	m_maxs = copyfrom.m_maxs;
}

AABB2::AABB2(float minX, float minY, float maxX, float maxY)
{
	m_maxs = Vec2(maxX, maxY);
	m_mins = Vec2(minX, minY);
}

AABB2::AABB2(Vec2 const& mins, Vec2 const& maxs)
{
	m_maxs = maxs;
	m_mins = mins;
}

AABB2::AABB2(Vec2 const& center, float height, float width)
{
	m_maxs = Vec2(center.x + width, center.y + height);
	m_mins = Vec2(center.x - width, center.y - height);
}

bool AABB2::IsPointInside(Vec2 const& point) const
{
	return (point.x <= m_maxs.x) && (point.y <= m_maxs.y) && (point.x >= m_mins.x) && (point.y >= m_mins.y);
}

Vec2 const AABB2::GetCenter() const
{
	return (m_mins + m_maxs) * 0.5f;
}

Vec2 const AABB2::GetDimensions() const
{
	return m_maxs - m_mins;
}

Vec2 const AABB2::GetHalfDimensions() const
{
	return GetDimensions() * 0.5f;
}

Vec2 const AABB2::GetNearestPoint(Vec2 const& referencePosition) const
{
	Vec2 clampedPosition = referencePosition;
	clampedPosition.x = Clamp(clampedPosition.x, m_mins.x, m_maxs.x);
	clampedPosition.y = Clamp(clampedPosition.y, m_mins.y, m_maxs.y);
	return clampedPosition;
}

Vec2 const AABB2::GetPointAtUV(Vec2 const& uv) const
{
	float pointX = Interpolate(m_mins.x, m_maxs.x, uv.x);
	float pointY = Interpolate(m_mins.y, m_maxs.y, uv.y);
	return Vec2(pointX, pointY);
}

Vec2 const AABB2::GetUVForPoint(Vec2 const& point) const
{
	float u = GetFractionWithinRange(point.x, m_mins.x, m_maxs.x);
	float v = GetFractionWithinRange(point.y, m_mins.y, m_maxs.y);
	return Vec2(u, v);
}

AABB2 const AABB2::GetAABBAtUVs(Vec2 const& uvMin, Vec2 const& uvMax)
{
	float minX = m_mins.x + (m_maxs.x - m_mins.x) * uvMin.x;
	float minY = m_mins.y + (m_maxs.y - m_mins.y) * uvMin.y;
	float maxX = m_maxs.x * uvMax.x;
	float maxY = m_maxs.y * uvMax.y;

	return AABB2(minX, minY, maxX, maxY);
}

std::vector<Vec2> AABB2::GetVertices() const
{
	std::vector < Vec2> verts;

	verts.push_back(m_mins);
	verts.push_back(Vec2(m_mins.x, m_maxs.y));
	verts.push_back(Vec2(m_maxs.x, m_mins.y));
	verts.push_back(m_maxs);

	return verts;
}

void AABB2::Translate(Vec2 const& translationToApply)
{
	m_mins += translationToApply;
	m_maxs += translationToApply;
}

void AABB2::SetCenter(Vec2 const& newCenter)
{
	Vec2 oldCenter = GetCenter();
	m_mins += (newCenter - oldCenter);
	m_maxs += (newCenter - oldCenter);
}

void AABB2::SetDimensions(Vec2 const& newDimensions)
{
	Vec2 center = GetCenter();
	m_maxs = Vec2(center.x + newDimensions.x * 0.5f, center.y + newDimensions.y * 0.5f);
	m_mins = Vec2(center.x - newDimensions.x * 0.5f, center.y - newDimensions.y * 0.5f);
}

AABB2 AABB2::Scale(float scale)
{
	AABB2 scaled;
	scaled.m_maxs = m_maxs * scale;
	scaled.m_mins = m_mins * (1 + (1 - scale));

	return scaled;
}

AABB2 AABB2::AddOffSet(float value)
{
	AABB2 result;
	result.m_maxs = m_maxs - Vec2(value, value);
	result.m_mins = m_mins + Vec2(value, value);

	return result;
}

void AABB2::StretchToIncludePoint(Vec2 const& point)
{
	if (point.x > m_maxs.x) m_maxs = Vec2(point.x, m_maxs.y);
	if (point.x < m_mins.x)	m_mins = Vec2(point.x, m_mins.y);
	if (point.y > m_maxs.y)	m_maxs = Vec2(m_maxs.x, point.y);
	if (point.y < m_mins.y)	m_mins = Vec2(m_mins.x, point.y);

}

std::vector<AABB2> AABB2::CreatePanes(int numPane, AABB2 box, int rows, int columns, float padX, float padY)
{
	int counter = numPane;

	std::vector<AABB2> panes;
	float width = box.m_maxs.x - box.m_mins.x;
	float height = box.m_maxs.y - box.m_mins.y;

	float widthStep = width / columns;
	float heightStep = height / rows;

	for (int c = 0; c < columns; c++)
	{
		if (counter <= 0)
		{
			break;
		}
		for (int r = 0; r < rows; r++)
		{
			if (counter <= 0)
			{
				break;
			}
			if (counter == 1 && (columns * rows - (rows - 1)) == numPane)
			{
				float maxX = width;
				float maxY = box.m_maxs.y - c * heightStep - padY;
				float minX = box.m_mins.x;
				float minY = box.m_mins.y;
				panes.push_back(AABB2(minX, minY, maxX, maxY));
			}
			else
			{
				float maxX = box.m_mins.x + (r + 1) * widthStep - padX;
				float maxY = box.m_maxs.y - c * heightStep - padY;
				float minX = maxX - widthStep + padX;
				float minY = maxY - heightStep + padY;
				panes.push_back(AABB2(minX, minY, maxX, maxY));
			}

			counter--;
		}
	}



	return panes;
}

AABB2 AABB2::ZERO_TO_ONE = AABB2(0.f, 0.f, 1.f, 1.f);
