#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Plane2.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include <vector>

class ConvexShape 
{
public:
	virtual Vec3 support(const Vec3& direction) const = 0;
};

class ConvexPoly2
{
public:
	ConvexPoly2() = default;
	ConvexPoly2(std::vector<Vec2> positions);

	void SetPositions(std::vector<Vec2> position);
	std::vector<Vec2> GetPositions() const;
	int GetSize() const;
	void SetPosition(int index, Vec2 newPos);
	void GetBoundingBox(Vec2& out_center, float& out_radius) const;
	static ConvexPoly2 CreateRandomConvexPoly(Vec2 center, float radius, FloatRange nextPointAngleRandomRange, RandomNumberGenerator rng = RandomNumberGenerator());
private:
	std::vector<Vec2> m_vertexPositions;
};

class ConvexHull2
{ 
public:
	ConvexHull2() = default;
	ConvexHull2(std::vector<Plane2> planes);
	explicit ConvexHull2(ConvexPoly2 poly);

	void SetPlanes(std::vector<Plane2> planes);
	std::vector<Plane2> GetPlanes() const;
	int GetSize() const;

	bool IsPointInside(Vec2 referencePoint);
private:
	std::vector<Plane2> m_boundingPlanes;
};