#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/RaycastUtils.hpp"



//-----------------------------------------------------------------------------------------------
//Angle Utilities

float ConvertDegreesToRadians(float degrees)
{
	return degrees * PI / 180.f;
}
//..............................
float ConvertRadiansToDegrees(float radians)
{
	return radians * 180.f / PI;
}

//..............................
double ConvertDegreesToRadiansDouble(double degrees)
{
	return degrees * 3.14159265359 / 180;
}

double ConvertRadiansToDegreesDouble(double radians)
{
	return radians * 180 / 3.14159265359;
}

//..............................
float CosDegrees(float degrees)
{
	return cosf(ConvertDegreesToRadians(degrees));
}

//..............................
float SinDegrees(float degrees)
{
	return sinf(ConvertDegreesToRadians(degrees));
}

float Atan2Degrees(float y, float x)
{
	return ConvertRadiansToDegrees(atan2f(y, x));
}

double CosDegreesDouble(double degrees)
{
	return cos(ConvertDegreesToRadiansDouble(degrees));
}
double SinDegreesDouble(double degrees)
{
	return sin(ConvertDegreesToRadiansDouble(degrees));
}

double Atan2DegreesDouble(double y, double x)
{
	return ConvertRadiansToDegreesDouble(atan2(y, x));
}

//..............................
float GetShortestAngularDispDegrees(float startDegrees, float endDegrees)
{
	float deltaDegrees = endDegrees - startDegrees;
	while (deltaDegrees > 180) deltaDegrees -= 360;
	while (deltaDegrees < -180) deltaDegrees += 360;
	return deltaDegrees;
}
//..............................
float GetTurnedTowardDegrees(float currentDegrees, float goalDegrees, float maxDeltaDegrees)
{
	float deltaDegrees = GetShortestAngularDispDegrees(currentDegrees, goalDegrees);
	return currentDegrees + Clamp(deltaDegrees, -maxDeltaDegrees, maxDeltaDegrees);
}
//..............................
float GetAngleDegreesBetweenVectors2D(Vec2 const& a, Vec2 const& b)
{
	float dotProduct = DotProduct2D(a, b);
	float aLength = a.GetLength();
	float blength = b.GetLength();

	float value = dotProduct / (aLength * blength);
	if (value >= 1.f)
	{
		value = 1.f;
	}
	if (value <= -1.f)
	{
		value = -1.f;
	}
	float angleRads = acosf(value);
	float angleDegrees = ConvertRadiansToDegrees(angleRads);

	return angleDegrees;
}

float GetAngleDegreesBetweenVectors3D(Vec3 const& a, Vec3 const& b)
{
	float dotProduct = DotProduct3D(a, b);
	float aLength = a.GetLength();
	float blength = b.GetLength();

	float angleRads = acosf(dotProduct / (aLength * blength));
	float angleDegrees = ConvertRadiansToDegrees(angleRads);

	return angleDegrees;
}

float GetNormalizedAngle(float angle)
{
	return static_cast<float>(abs(static_cast<int>(angle) % 360));
}

//-----------------------------------------------------------------------------------------------
//Basic 2D and 3D Utilities

float GetDistance2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float x = positionB.x - positionA.x;
	float y = positionB.y - positionA.y;
	return sqrtf((x * x) + (y * y));
}
//..............................
float GetDistanceSquared2D(Vec2 const& positionA, Vec2 const& positionB)
{
	float x = positionB.x - positionA.x;
	float y = positionB.y - positionA.y;
	return (x * x) + (y * y);
}
//..............................
float GetDistance3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float x = positionB.x - positionA.x;
	float y = positionB.y - positionA.y;
	float z = positionB.z - positionA.z;
	return sqrtf((x * x) + (y * y) + (z * z));
}
//..............................
float GetDistanceSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float x = positionB.x - positionA.x;
	float y = positionB.y - positionA.y;
	float z = positionB.z - positionA.z;
	return (x * x) + (y * y) + (z * z);
}

double GetDistanceSquared3D_Double(DoubleVec3 const& positionA, DoubleVec3 const& positionB)
{
	double x = positionB.x - positionA.x;
	double y = positionB.y - positionA.y;
	double z = positionB.z - positionA.z;
	return (x * x) + (y * y) + (z * z);
}

//..............................
float GetDistanceXY3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float x = positionB.x - positionA.x;
	float y = positionB.y - positionA.y;
	return sqrtf((x * x) + (y * y));
}
//..............................
float GetDistanceXYSquared3D(Vec3 const& positionA, Vec3 const& positionB)
{
	float x = positionB.x - positionA.x;
	float y = positionB.y - positionA.y;
	return (x * x) + (y * y);
}
//..............................
int GetTaxicabDistance2D(IntVec2 const& pointA, IntVec2 const& pointB)
{
	return abs(pointB.x - pointA.x) + abs(pointB.y - pointA.y);
}
//..............................
float GetProjectedLength2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 normalizedVectorToProjectOnto = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct2D(vectorToProject, normalizedVectorToProjectOnto);
	return projectedLength;
}
//..............................
Vec2 const GetProjectedOnto2D(Vec2 const& vectorToProject, Vec2 const& vectorToProjectOnto)
{
	Vec2 normalizedVectorToProjectOnto = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct2D(vectorToProject, vectorToProjectOnto);
	Vec2 projectedVector = normalizedVectorToProjectOnto * projectedLength;
	return projectedVector;
}

Vec3 const GetProjectedOnto3D(Vec3 const& vectorToProject, Vec3 const& vectorToProjectOnto)
{
	Vec3 normalizedVectorToProjectOnto = vectorToProjectOnto.GetNormalized();
	float projectedLength = DotProduct3D(vectorToProject, normalizedVectorToProjectOnto);
	Vec3 projectedVector = normalizedVectorToProjectOnto * projectedLength;
	return projectedVector;
}

DoubleVec3 const GetProjectedOnto3D_Double(DoubleVec3 const& vectorToProject, DoubleVec3 const& vectorToProjectOnto)
{
	DoubleVec3 normalizedVectorToProjectOnto = vectorToProjectOnto.GetNormalized();
	double projectedLength = DotProduct3D_Double(vectorToProject, normalizedVectorToProjectOnto);
	DoubleVec3 projectedVector = normalizedVectorToProjectOnto * projectedLength;
	return projectedVector;
}

bool IsPointInsideZHexagon3D(Vec3 const& point, Vec3 const& center, float radius)
{
	Vec3 hexPoints[6];
	GetAllPointsZHexagon3D(hexPoints, center, radius * (2 / sqrtf(3)));

	Vec3 pointOnHexPlane = Vec3(point.x, point.y, center.z);

	for (int i = 0; i < 6; i++)
	{
		Vec3 current = hexPoints[i];
		Vec3 next = hexPoints[(i + 1) % 6];

		Vec3 edge = next - current;
		Vec3 toPoint = pointOnHexPlane - current;
		Vec3 normal = edge.Cross(toPoint);
		if (normal.z < 0)
		{
			return false;
		}
	}
	return true;
}

void GetAllPointsZHexagon3D(Vec3* out_Points, Vec3 const& center, float radius)
{
	const int NUM_SIDES = 6;
	const float DEGREES_PER_SIDE = 360.0f / NUM_SIDES;

	for (int i = 0; i < NUM_SIDES; i++)
	{
		float angleDegrees = i * DEGREES_PER_SIDE;
		float sinAngle = SinDegrees(angleDegrees);
		float cosAngle = CosDegrees(angleDegrees);

		float x = center.x + radius * cosAngle;
		float y = center.y + radius * sinAngle;

		out_Points[i] = Vec3(x, y, center.z);
	}
}

//..............................
bool IsPointInsideDisc2D(Vec2 const& point, Vec2 const& discCenter, float discRadius)
{
	float distanceSquared = GetDistanceSquared2D(point, discCenter);
	if (distanceSquared < discRadius * discRadius)
	{
		return true;
	}
	return false;
}

bool IsPointInsidePolygon2D(Vec2 const& point, std::vector<Vec2> polygon)
{
	bool inside = false;
	size_t n = polygon.size();

	for (size_t i = 0, j = n - 1; i < n; j = i++)
	{
		if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
			(point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) /
				(polygon[j].y - polygon[i].y) + polygon[i].x))
		{
			inside = !inside;
		}
	}

	return inside;
}

bool IsPointInsideAABB2D(Vec2 const& point, AABB2 const& box)
{
	return box.IsPointInside(point);
}
bool IsPointInsideCapsule2D(Vec2 const& point, Capsule2 const& capsule)
{
	return IsPointInsideCapsule2D(point, capsule.m_start, capsule.m_end, capsule.m_radius);
}
bool IsPointInsideCapsule2D(Vec2 const& point, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 obbCenter = (boneEnd + boneStart) * 0.5f;
	Vec2 obbIBasis = (boneEnd - boneStart).GetNormalized();
	Vec2 obbHalfDimension = Vec2((boneEnd - boneStart).GetLength() * 0.5f, radius);
	OBB2 body = OBB2(obbCenter, obbIBasis, obbHalfDimension);
	if (IsPointInsideOBB2D(point, body)) return true;
	if (IsPointInsideDisc2D(point, boneStart, radius)) return true;
	if (IsPointInsideDisc2D(point, boneEnd, radius)) return true;
	return false;
}
bool IsPointInsideOBB2D(Vec2 const& point, OBB2 const& oriendtedBox)
{
	return oriendtedBox.IsPointInside(point);
}
//..............................
bool IsPointInsideOrientedSector2D(Vec2 const& point, Vec2 const& sectorTip, float sectorForwardDegrees, float sectorApertureDegrees, float sectorRadius)
{
	Vec2  sectorForwardNormal = Vec2::MakeFromPolarDegrees(sectorForwardDegrees);
	return IsPointInsideDirectedSector2D(point, sectorTip, sectorForwardNormal, sectorApertureDegrees, sectorRadius);
}
//..............................
bool IsPointInsideDirectedSector2D(Vec2 const& point, Vec2 const& sectorTip, Vec2 const& sectorForwardNormal, float sectorApertureDegrees, float sectorRadius)
{
	if (!IsPointInsideDisc2D(point, sectorTip, sectorRadius)) return false;

	Vec2 pointVector = point - sectorTip;
	float angleBetweenPointAndSFN = GetAngleDegreesBetweenVectors2D(sectorForwardNormal, pointVector);
	if (angleBetweenPointAndSFN < sectorApertureDegrees * 0.5f)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
//Geometric Query Utilities

bool DoDiscsOverlap2D(Vec2 const& centerA, float radiusA, Vec2 const& centerB, float radiusB)
{
	float distanceSquaredTwoCenters = GetDistanceSquared2D(centerA, centerB);
	return distanceSquaredTwoCenters < (radiusA + radiusB) * (radiusA + radiusB);
}
bool DoAABBsOverlap3D(AABB3 const& boxA, AABB3 const& boxB)
{
	if (boxA.m_maxs.x <= boxB.m_mins.x || boxA.m_mins.x >= boxB.m_maxs.x)
	{
		return false;
	}
	if (boxA.m_maxs.y <= boxB.m_mins.y || boxA.m_mins.y >= boxB.m_maxs.y)
	{
		return false;
	}
	if (boxA.m_maxs.z <= boxB.m_mins.z || boxA.m_mins.z >= boxB.m_maxs.z)
	{
		return false;
	}
	return true;
}

bool DoAABBsOverlap3D_Double(DoubleAABB3 const& boxA, DoubleAABB3 const& boxB)
{
	if (boxA.m_maxs.x <= boxB.m_mins.x || boxA.m_mins.x >= boxB.m_maxs.x)
	{
		return false;
	}
	if (boxA.m_maxs.y <= boxB.m_mins.y || boxA.m_mins.y >= boxB.m_maxs.y)
	{
		return false;
	}
	if (boxA.m_maxs.z <= boxB.m_mins.z || boxA.m_mins.z >= boxB.m_maxs.z)
	{
		return false;
	}
	return true;
}

bool IsAABBInside(DoubleAABB3 const& parent, DoubleAABB3 const& check)
{
	if (check.m_mins.x < parent.m_mins.x || check.m_maxs.x > parent.m_maxs.x)
	{
		return false;
	}
	if (check.m_mins.y < parent.m_mins.y || check.m_maxs.y > parent.m_maxs.y)
	{
		return false;
	}
	if (check.m_mins.z < parent.m_mins.z || check.m_maxs.z > parent.m_maxs.z)
	{
		return false;
	}
	return true;
}

//..............................
bool DoSpheresOverlap3D(Vec3 const& centerA, float radiusA, Vec3 const& centerB, float radiusB)
{
	float distanceSquaredTwoCenters = GetDistanceSquared3D(centerA, centerB);
	return distanceSquaredTwoCenters < (radiusA + radiusB) * (radiusA + radiusB);
}

bool DoZCylindersOverlap3D(Vec2 const& cylinderAcenterXY, float cylinderARadius, FloatRange const& cylinderAMinMaxZ, Vec2 const& cylinderBcenterXY, float cylinderBRadius, FloatRange const& cylinderBMinMaxZ)
{
	if (cylinderAMinMaxZ.m_max <= cylinderBMinMaxZ.m_min || cylinderAMinMaxZ.m_min >= cylinderBMinMaxZ.m_max)
	{
		return false;
	}
	Vec3 cylinderACenter = Vec3(cylinderAcenterXY.x, cylinderAcenterXY.y, (cylinderAMinMaxZ.m_max + cylinderAMinMaxZ.m_min) / 2.f);
	Vec3 nearestPointOnCylinderB = GetNearestPointOnZCynlinder3D(cylinderACenter, cylinderBcenterXY, cylinderBMinMaxZ, cylinderBRadius);
	if (!IsPointInsideZCylinder3D(nearestPointOnCylinderB, cylinderAcenterXY, cylinderAMinMaxZ, cylinderARadius))
	{
		return false;
	}
	return true;
}
bool DoSphereAndAABBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, AABB3 const& box)
{
	if (box.m_maxs.x <= sphereCenter.x - sphereRadius
		|| box.m_mins.x >= sphereCenter.x + sphereRadius
		|| box.m_maxs.y <= sphereCenter.y - sphereRadius
		|| box.m_mins.y >= sphereCenter.y + sphereRadius
		|| box.m_maxs.z <= sphereCenter.z - sphereRadius
		|| box.m_mins.z >= sphereCenter.z + sphereRadius)
	{
		return false;
	}

	Vec3 nearestPoint = GetNearestPointOnAABB3D(sphereCenter, box);
	return IsPointInsideSphere3D(nearestPoint, sphereCenter, sphereRadius);
}

bool DoSphereAndOBBOverlap3D(Vec3 const& sphereCenter, float sphereRadius, OBB3 const& obb)
{
	Vec3 kBasisNormal = CrossProduct3D(obb.m_iBasisNormal, obb.m_jBasisNormal);

	Mat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	Mat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	AABB3 aabb;
	aabb.m_mins = Vec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = Vec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	Vec3 sphereCenterLocal = obbWtoL.TransformPosition3D(sphereCenter);

	return DoSphereAndAABBOverlap3D(sphereCenterLocal, sphereRadius, aabb);
}

bool DoZCylinderAndAABBOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange const& cylinderMinMaxZ, AABB3 const& box)
{
	if (box.m_maxs.z <= cylinderMinMaxZ.m_min || box.m_mins.z >= cylinderMinMaxZ.m_max)
	{
		return false;
	}
	AABB2 topViewBox = AABB2(Vec2(box.m_mins.x, box.m_mins.y), Vec2(box.m_maxs.x, box.m_maxs.y));
	Vec2 point = GetNearestPointOnAABB2D(cylinderCenterXY, topViewBox);
	if (!IsPointInsideDisc2D(point, cylinderCenterXY, cylinderRadius))
	{
		return false;
	}
	return true;
}
bool DoZCylinderAndSphereOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange const& cylinderMinMaxZ, Vec3 const& sphereCenter, float sphereRadius)
{
	if (sphereCenter.z + sphereRadius <= cylinderMinMaxZ.m_min || sphereCenter.z - sphereRadius >= cylinderMinMaxZ.m_max)
	{
		return false;
	}
	Vec3 nearestPointZCyl = GetNearestPointOnZCynlinder3D(sphereCenter, cylinderCenterXY, cylinderMinMaxZ, cylinderRadius);
	if (!IsPointInsideSphere3D(nearestPointZCyl, sphereCenter, sphereRadius))
	{
		return false;
	}
	return true;
}

bool DoAABBAndPlaneOverlap3D(AABB3 const& box, Plane3 const& plane)
{
	float dotList[8];
	Vec3 allPoints[8];
	box.GetCornerPoints(allPoints);
	for (size_t i = 0; i < 8; i++)
	{
		dotList[i] = DotProduct3D(allPoints[i], plane.m_normal) - plane.m_distanceFromOrigin;
	}
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = i + 1; j < 8; j++)
		{
			if (dotList[i] * dotList[j] < 0.f)
			{
				return true;
			}
		}
	}
	return false;
}

bool DoSphereAndPlaneOverlap3D(Vec3 const& center, float radius, Plane3 const& plane)
{
	float distanceToPlane = DotProduct3D(plane.m_normal, center) - plane.m_distanceFromOrigin;
	return fabsf(distanceToPlane) <= radius;
}

bool DoZCylinderAndPlaneOverlap3D(Vec2 const& cylinderCenterXY, float cylinderRadius, FloatRange const& cylinderMinMaxZ, Plane3 const& plane)
{
	Vec3 center = Vec3(cylinderCenterXY.x, cylinderCenterXY.y, (cylinderMinMaxZ.m_min + cylinderMinMaxZ.m_max) * 0.5f);
	Vec3 nearestPointOnPlane = plane.GetNearestPoint(center);
	return IsPointInsideZCylinder3D(nearestPointOnPlane, cylinderCenterXY, cylinderMinMaxZ, cylinderRadius);
}

bool DoOBBsOverlap3D(OBB3 const& boxA, OBB3 const& boxB)
{
	float ra, rb;
	Mat44 R, AbsR;

	Vec3 boxA_u[3];
	boxA_u[0] = boxA.m_iBasisNormal;
	boxA_u[1] = boxA.m_jBasisNormal;
	boxA_u[2] = boxA_u[0].Cross(boxA_u[1]);

	float boxA_e[3];
	boxA_e[0] = boxA.m_halfDimensions.x;
	boxA_e[1] = boxA.m_halfDimensions.y;
	boxA_e[2] = boxA.m_halfDimensions.z;

	Vec3 boxB_u[3];
	boxB_u[0] = boxB.m_iBasisNormal;
	boxB_u[1] = boxB.m_jBasisNormal;
	boxB_u[2] = boxB_u[0].Cross(boxB_u[1]);

	float boxB_e[3];
	boxB_e[0] = boxB.m_halfDimensions.x;
	boxB_e[1] = boxB.m_halfDimensions.y;
	boxB_e[2] = boxB.m_halfDimensions.z;


	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			R.m_values[i * 4 + j] = DotProduct3D(boxA_u[i], boxB_u[j]);
		}
	}

	Vec3 dir = boxB.m_center - boxA.m_center;
	float t[3];
	t[0] = DotProduct3D(dir, boxA_u[0]);
	t[1] = DotProduct3D(dir, boxA_u[1]);
	t[2] = DotProduct3D(dir, boxA_u[2]);

	for (size_t i = 0; i < 6; i++)
	{
		AbsR.m_values[i] = fabsf(R.m_values[i]);
	}

	// Test axes L = A0, L = A1, L = A2
	for (size_t i = 0; i < 3; i++)
	{
		ra = boxA_e[i];
		rb = boxB_e[0] + AbsR.m_values[i * 4 + 0] + boxB_e[1] * AbsR.m_values[i * 4 + 1] + boxB_e[2] * AbsR.m_values[i * 4 + 2];
		if (fabsf(t[i] > ra + rb)) return false;
	}

	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++)
	{
		ra = boxA_e[0] * AbsR.m_values[0 * 4 + i] + boxA_e[1] * AbsR.m_values[1 * 4 + i] + boxA_e[2] * AbsR.m_values[2 * 4 + i];
		rb = boxB_e[i];
		if (fabsf(t[0] * R.m_values[0 * 4 + i] + t[1] * R.m_values[1 * 4 + i] + t[2] * R.m_values[2 * 4 + i]) > ra + rb) return false;
	}

	// Test axis L = A0 x B0
	ra = boxA_e[1] * AbsR.m_values[2 * 4 + 0] + boxA_e[2] + AbsR.m_values[1 * 4 + 0];
	rb = boxB_e[1] * AbsR.m_values[0 * 4 + 2] + boxB_e[2] + AbsR.m_values[0 * 4 + 1];
	if (fabsf(t[2] * R.m_values[1 * 4 + 0] - t[1] * R.m_values[2 * 4 + 0]) > ra + rb) return false;



	return true;
}

bool DoOBBAndPlaneOverlap3D(OBB3 const& box, Plane3 const& plane)
{
	float dotList[8];
	Vec3 allPoints[8];
	box.GetCornerPoints(allPoints);
	for (size_t i = 0; i < 8; i++)
	{
		dotList[i] = DotProduct3D(allPoints[i], plane.m_normal) - plane.m_distanceFromOrigin;
	}
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = i + 1; j < 8; j++)
		{
			if (dotList[i] * dotList[j] < 0.f)
			{
				return true;
			}
		}
	}
	return false;
}

bool DoOBBAndAABBOverlap3D(OBB3 const& obb, AABB3 const& aabb)
{
	Vec3 obbPoints[8];
	obb.GetCornerPoints(obbPoints);

	float maxX = FloatMax(obbPoints[0].x, FloatMax(obbPoints[1].x, FloatMax(obbPoints[2].x, FloatMax(obbPoints[3].x, FloatMax(obbPoints[4].x, FloatMax(obbPoints[5].x, FloatMax(obbPoints[6].x, obbPoints[7].x)))))));
	float minX = FloatMin(obbPoints[0].x, FloatMin(obbPoints[1].x, FloatMin(obbPoints[2].x, FloatMin(obbPoints[3].x, FloatMin(obbPoints[4].x, FloatMin(obbPoints[5].x, FloatMin(obbPoints[6].x, obbPoints[7].x)))))));
	if (aabb.m_mins.x > maxX || aabb.m_maxs.x < minX)
	{
		return false;
	}

	float maxY = FloatMax(obbPoints[0].y, FloatMax(obbPoints[1].y, FloatMax(obbPoints[2].y, FloatMax(obbPoints[3].y, FloatMax(obbPoints[4].y, FloatMax(obbPoints[5].y, FloatMax(obbPoints[6].y, obbPoints[7].y)))))));
	float minY = FloatMin(obbPoints[0].y, FloatMin(obbPoints[1].y, FloatMin(obbPoints[2].y, FloatMin(obbPoints[3].y, FloatMin(obbPoints[4].y, FloatMin(obbPoints[5].y, FloatMin(obbPoints[6].y, obbPoints[7].y)))))));
	if (aabb.m_mins.y > maxY || aabb.m_maxs.y < minY)
	{
		return false;
	}

	float maxZ = FloatMax(obbPoints[0].z, FloatMax(obbPoints[1].z, FloatMax(obbPoints[2].z, FloatMax(obbPoints[3].z, FloatMax(obbPoints[4].z, FloatMax(obbPoints[5].z, FloatMax(obbPoints[6].z, obbPoints[7].z)))))));
	float minZ = FloatMin(obbPoints[0].z, FloatMin(obbPoints[1].z, FloatMin(obbPoints[2].z, FloatMin(obbPoints[3].z, FloatMin(obbPoints[4].z, FloatMin(obbPoints[5].z, FloatMin(obbPoints[6].z, obbPoints[7].z)))))));
	if (aabb.m_mins.z > maxZ || aabb.m_maxs.z < minZ)
	{
		return false;
	}

	return true;
}

bool DoCapsuleAndAABBOverlap3D(Capsule3 const& capsule, AABB3 const& box)
{
	bool returnValue = false;
	returnValue = DoSphereAndAABBOverlap3D(capsule.m_start, capsule.m_radius, box);
	returnValue |= DoSphereAndAABBOverlap3D(capsule.m_end, capsule.m_radius, box);

	// Best Possible Intersects
	float tValues[6];
	float bestTValue = FLT_MAX;
	float bestDistanceSquared = FLT_MAX;
	Vec3 bestIntersectPoint = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vec3 bestNearestPointAABB = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vec3 axis = capsule.GetAxis();
	float rayZScale = 1.0f / axis.z;
	float rayYScale = 1.0f / axis.y;
	float rayXScale = 1.0f / axis.x;
	tValues[0] = ((box.m_maxs.z - capsule.m_start.z) * rayZScale);
	tValues[1] = ((box.m_mins.z - capsule.m_start.z) * rayZScale);
	tValues[2] = ((box.m_maxs.y - capsule.m_start.y) * rayYScale);
	tValues[3] = ((box.m_mins.y - capsule.m_start.y) * rayYScale);
	tValues[4] = ((box.m_maxs.x - capsule.m_start.x) * rayXScale);
	tValues[5] = ((box.m_mins.x - capsule.m_start.x) * rayXScale);

	for (float tValue : tValues)
	{
		if (tValue >= 0.0f && tValue <= 1.0f)
		{
			Vec3 intersectPoint = capsule.m_start + (tValue * axis);
			Vec3 nearestPointAABB = box.GetNearestEdgePosition(intersectPoint);
			float distanceSquared = GetDistanceSquared3D(intersectPoint, nearestPointAABB);
			if (distanceSquared < bestDistanceSquared)
			{
				bestDistanceSquared = distanceSquared;
				bestIntersectPoint = intersectPoint;
				bestNearestPointAABB = nearestPointAABB;
				bestTValue = tValue;
			}
		}
	}

	// Doesn't hit
	if (bestTValue < -1.0f || bestTValue > 2.0f)
	{
		return returnValue;
	}

	Vec3 center = box.GetCenter();
	Vec3 nearestPointBone = GetNearestPointOnLineSegment3D(capsule.m_start, capsule.m_end, bestNearestPointAABB);
	bestNearestPointAABB = box.GetNearestEdgePosition(nearestPointBone);

	if (IsPointInsideAABB3D(nearestPointBone, box))
	{
		return true;
	}
	else
	{
		Vec3 displacement = nearestPointBone - bestNearestPointAABB;
		float distanceSquared = displacement.GetLengthSquared();
		if (distanceSquared < capsule.m_radius * capsule.m_radius)
		{
			return true;
		}
	}

	return returnValue;
}

CollisionInfo DoCapsuleAndAABBOverlap3D_Info(DoubleCapsule3 const& capsule, DoubleAABB3 const& box)
{
	CollisionInfo returnValue;

	if (DoSphereAndAABBOverlap3D_Double(capsule.m_start, capsule.m_radius, box))
	{
		returnValue.isColliding = true;
		returnValue.contactPoint = GetNearestPointOnAABB3D_Double(capsule.m_start, box);
		return returnValue;
	}
	if (DoSphereAndAABBOverlap3D_Double(capsule.m_end, capsule.m_radius, box))
	{
		returnValue.isColliding = true;
		returnValue.contactPoint = GetNearestPointOnAABB3D_Double(capsule.m_end, box);
		return returnValue;
	}

	// Best Possible Intersects
	double tValues[6];
	double bestTValue = FLT_MAX;
	double bestDistanceSquared = FLT_MAX;
	DoubleVec3 bestIntersectPoint = DoubleVec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	DoubleVec3 bestNearestPointAABB = DoubleVec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	DoubleVec3 axis = capsule.GetAxis();
	double rayZScale = 1.0f / axis.z;
	double rayYScale = 1.0f / axis.y;
	double rayXScale = 1.0f / axis.x;
	tValues[0] = ((box.m_maxs.z - capsule.m_start.z) * rayZScale);
	tValues[1] = ((box.m_mins.z - capsule.m_start.z) * rayZScale);
	tValues[2] = ((box.m_maxs.y - capsule.m_start.y) * rayYScale);
	tValues[3] = ((box.m_mins.y - capsule.m_start.y) * rayYScale);
	tValues[4] = ((box.m_maxs.x - capsule.m_start.x) * rayXScale);
	tValues[5] = ((box.m_mins.x - capsule.m_start.x) * rayXScale);

	for (double tValue : tValues)
	{
		if (tValue >= 0.0f && tValue <= 1.0f)
		{
			DoubleVec3 intersectPoint = capsule.m_start + (tValue * axis);
			DoubleVec3 nearestPointAABB = box.GetNearestEdgePosition(intersectPoint);
			double distanceSquared = GetDistanceSquared3D(intersectPoint, nearestPointAABB);
			if (distanceSquared < bestDistanceSquared)
			{
				bestDistanceSquared = distanceSquared;
				bestIntersectPoint = intersectPoint;
				bestNearestPointAABB = nearestPointAABB;
				bestTValue = tValue;
			}
		}
	}

	// Doesn't hit
	if (bestTValue < -1.0 || bestTValue > 2.0)
	{
		return returnValue;
	}

	DoubleVec3 center = box.GetCenter();
	DoubleVec3 nearestPointBone = GetNearestPointOnLineSegment3D(capsule.m_start, capsule.m_end, bestNearestPointAABB);
	bestNearestPointAABB = box.GetNearestEdgePosition(nearestPointBone);

	if (IsPointInsideAABB3D(nearestPointBone, box))
	{
		returnValue.isColliding = true;
		returnValue.contactPoint = bestNearestPointAABB;
		returnValue.normal = (bestNearestPointAABB - box.GetCenter()).GetNormalized();
		return returnValue;
	}
	else
	{
		DoubleVec3 displacement = nearestPointBone - bestNearestPointAABB;
		double distanceSquared = displacement.GetLengthSquared();
		if (distanceSquared < capsule.m_radius * capsule.m_radius)
		{
			returnValue.isColliding = true;
			returnValue.contactPoint = bestNearestPointAABB;
			returnValue.normal = (bestNearestPointAABB - box.GetCenter()).GetNormalized();
			return returnValue;
		}
	}

	return returnValue;
}

CollisionInfo DoCapsuleAndOBBOverlap3D_Info(DoubleCapsule3 const& capsule, DoubleOBB3 const& obb)
{
	DoubleVec3 kBasisNormal = CrossProduct3D_Double(obb.m_iBasisNormal, obb.m_jBasisNormal);

	DoubleMat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	DoubleMat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	DoubleAABB3 aabb;
	aabb.m_mins = DoubleVec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = DoubleVec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	DoubleCapsule3 localCapsule;
	localCapsule.m_radius = capsule.m_radius;
	localCapsule.m_start = obbWtoL.TransformPosition3D(capsule.m_start);
	localCapsule.m_end = obbWtoL.TransformPosition3D(capsule.m_end);

	CollisionInfo colLocalAABB = DoCapsuleAndAABBOverlap3D_Info(localCapsule, aabb);
	colLocalAABB.contactPoint = obbLtoW.TransformPosition3D(colLocalAABB.contactPoint);
	colLocalAABB.normal = (colLocalAABB.contactPoint - obb.m_center).GetNormalized();
	return colLocalAABB;
}

CollisionInfo DoCapsuleAndSphereOverlap3D_Info(DoubleCapsule3 const& capsule, DoubleVec3 const& center, double radius)
{
	CollisionInfo returnValue;

	DoubleVec3 nearestPointToBone = GetNearestPointOnLineSegment3D_Double(capsule.m_start, capsule.m_end, center);
	DoubleVec3 fromSphereCenterToBone = nearestPointToBone - center;
	double distanceFromSphereToBoneSquared = fromSphereCenterToBone.GetLengthSquared();
	if (distanceFromSphereToBoneSquared < (capsule.m_radius + radius) * (capsule.m_radius + radius))
	{
		returnValue.isColliding = true;
		returnValue.contactPoint = GetNearestPointOnSphere3D_Double(nearestPointToBone, center, radius);
		returnValue.normal = (returnValue.contactPoint - center).GetNormalized();
	}

	return returnValue;
}

CollisionInfo DoCapsulesOverlap3D_Info(DoubleCapsule3 const& capsuleA, DoubleCapsule3 const& capsuleB)
{
	CollisionInfo returnValue;

	DoubleVec3 capPoints[2];
	Optimized_GetNearestPointsBetweenLines3D_Double(capPoints, capsuleA.GetBone(), capsuleB.GetBone());

	if (!capPoints)
	{
		return returnValue;
	}
	DoubleVec3 dist2Points = capPoints[0] - capPoints[1];
	double dist2PointsLengthSquared = dist2Points.GetLengthSquared();


	if (dist2PointsLengthSquared < (capsuleA.m_radius + capsuleB.m_radius) * (capsuleA.m_radius + capsuleB.m_radius))
	{
		returnValue.isColliding = true;

		double dist = sqrt(dist2PointsLengthSquared);
		double offset = capsuleA.m_radius + capsuleB.m_radius - dist;

		returnValue.contactPoint = capPoints[1] + dist2Points.GetNormalized() * (capsuleB.m_radius - offset * 0.5);
		returnValue.normal = (capPoints[0] - capPoints[1]).GetNormalized();
	}

	return returnValue;
}

CollisionInfo DoSpheresOverlap3D_Info(DoubleVec3 const& centerA, double radiusA, DoubleVec3 const& centerB, double radiusB)
{
	CollisionInfo returnValue;

	if (DoSpheresOverlap3D_Double(centerA, radiusA, centerB, radiusB))
	{
		returnValue.isColliding = true;

		DoubleVec3 AtoB = centerB - centerA;
		double offset = (radiusA + radiusB) - AtoB.GetLength();
		returnValue.contactPoint = centerA + AtoB.GetNormalized() * (radiusA - offset * 0.5);
	}

	return returnValue;
}

bool DoCapsuleAndOBBOverlap3D(Capsule3 const& capsule, OBB3 const& obb)
{
	Vec3 kBasisNormal = CrossProduct3D(obb.m_iBasisNormal, obb.m_jBasisNormal);

	Mat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	Mat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	AABB3 aabb;
	aabb.m_mins = Vec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = Vec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	Capsule3 localCapsule;
	localCapsule.m_radius = capsule.m_radius;
	localCapsule.m_start = obbWtoL.TransformPosition3D(capsule.m_start);
	localCapsule.m_end = obbWtoL.TransformPosition3D(capsule.m_end);

	return DoCapsuleAndAABBOverlap3D(localCapsule, aabb);
}

bool DoCapsuleAndPlaneOverlap3D(Capsule3 const& capsule, Plane3 const& plane)
{

	Vec3 tip = capsule.m_start - capsule.GetAxisNormal() * capsule.m_radius;
	Vec3 tail = capsule.m_end + capsule.GetAxisNormal() * capsule.m_radius;

	float dotTip = DotProduct3D(tip, plane.m_normal) - plane.m_distanceFromOrigin;
	float dotTail = DotProduct3D(tail, plane.m_normal) - plane.m_distanceFromOrigin;

	return dotTip * dotTail < 0;
}

bool DoCapsuleAndSphereOverlap3D(Capsule3 const& capsule, Vec3 const& sphereCenter, float sphereRadius)
{
	Vec3 nearestPoint = capsule.GetNearestPoint(sphereCenter);
	return IsPointInsideSphere3D(nearestPoint, sphereCenter, sphereRadius);
}

bool DoCapsulesOverlap3D(Capsule3 const& capsuleA, Capsule3 const& capsuleB)
{
	std::vector<Vec3> nearestPointsOn2Bones = GetNearestPointsBetweenLines3D(capsuleA.GetBone(), capsuleB.GetBone());
	if (nearestPointsOn2Bones.empty())
	{
		return false;
	}
	Vec3 dist2Points = nearestPointsOn2Bones[0] - nearestPointsOn2Bones[1];
	float dist2PointsLengthSquared = dist2Points.GetLengthSquared();

	return dist2PointsLengthSquared < (capsuleA.m_radius + capsuleB.m_radius) * (capsuleA.m_radius + capsuleB.m_radius);
}

bool DoSpheresOverlap3D_Double(DoubleVec3 const& centerA, double radiusA, DoubleVec3 const& centerB, double radiusB)
{
	double distanceSquaredTwoCenters = GetDistanceSquared3D_Double(centerA, centerB);
	return distanceSquaredTwoCenters < (radiusA + radiusB) * (radiusA + radiusB);
}

bool DoSphereAndAABBOverlap3D_Double(DoubleVec3 const& sphereCenter, double sphereRadius, DoubleAABB3 const& box)
{
	if (box.m_maxs.x <= sphereCenter.x - sphereRadius
		|| box.m_mins.x >= sphereCenter.x + sphereRadius
		|| box.m_maxs.y <= sphereCenter.y - sphereRadius
		|| box.m_mins.y >= sphereCenter.y + sphereRadius
		|| box.m_maxs.z <= sphereCenter.z - sphereRadius
		|| box.m_mins.z >= sphereCenter.z + sphereRadius)
	{
		return {};
	}

	CollisionInfo result;
	DoubleVec3 nearestPoint = GetNearestPointOnAABB3D_Double(sphereCenter, box);
	return IsPointInsideSphere3D_Double(nearestPoint, sphereCenter, sphereRadius);
}

bool DoSphereAndPlaneOverlap3D_Double(DoubleVec3 const& center, double radius, DoublePlane3 const& plane)
{
	double distanceToPlane = DotProduct3D_Double(plane.m_normal, center) - plane.m_distanceFromOrigin;
	return fabs(distanceToPlane) <= radius;
}

bool DoCapsuleAndSphereOverlap3D_Double(DoubleCapsule3 const& capsule, DoubleVec3 const& sphereCenter, double sphereRadius)
{
	CollisionInfo result;
	DoubleVec3 nearestPoint = capsule.GetNearestPoint(sphereCenter);
	return IsPointInsideSphere3D_Double(nearestPoint, sphereCenter, sphereRadius);
}

bool DoCapsulesOverlap3D_Double(DoubleCapsule3 const& capsule, DoubleCapsule3 const& otherCapsule)
{
	DoubleVec3 capPoints[2];
	Optimized_GetNearestPointsBetweenLines3D_Double(capPoints, capsule.GetBone(), otherCapsule.GetBone());

	if (!capPoints)
	{
		return false;
	}
	Vec3 dist2Points = capPoints[0] - capPoints[1];
	float dist2PointsLengthSquared = dist2Points.GetLengthSquared();

	return dist2PointsLengthSquared < (capsule.m_radius + otherCapsule.m_radius) * (capsule.m_radius + otherCapsule.m_radius);
}

bool DoSphereAndOBBOverlap3D_Double(DoubleVec3 const& sphereCenter, double sphereRadius, DoubleOBB3 const& obb)
{
	DoubleVec3 kBasisNormal = CrossProduct3D_Double(obb.m_iBasisNormal, obb.m_jBasisNormal);

	DoubleMat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	DoubleMat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	DoubleAABB3 aabb;
	aabb.m_mins = DoubleVec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = DoubleVec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	DoubleVec3 sphereCenterLocal = obbWtoL.TransformPosition3D(sphereCenter);

	return DoSphereAndAABBOverlap3D_Double(sphereCenterLocal, sphereRadius, aabb);
}

bool DoCapsuleAndPlaneOverlap3D_Double(DoubleCapsule3 const& capsule, DoublePlane3 const& plane)
{
	DoubleVec3 tip = capsule.m_start - capsule.GetAxisNormal() * capsule.m_radius;
	DoubleVec3 tail = capsule.m_end + capsule.GetAxisNormal() * capsule.m_radius;

	double dotTip = DotProduct3D_Double(tip, plane.m_normal) - plane.m_distanceFromOrigin;
	double dotTail = DotProduct3D_Double(tail, plane.m_normal) - plane.m_distanceFromOrigin;

	return dotTip * dotTail < 0;
}

bool DoAABBsOverlap2D(AABB2 const& boxA, AABB2 const& boxB)
{
	bool xOverlap = (boxA.m_mins.x < boxB.m_maxs.x) && (boxA.m_maxs.x > boxB.m_mins.x);
	bool yOverlap = (boxA.m_mins.y < boxB.m_maxs.y) && (boxA.m_maxs.y > boxB.m_mins.y);
	return xOverlap && yOverlap;
}

bool DoCapsuleAndPlaneOverlap2D(Capsule2 const& capsule, Vec2 planeStart, Vec2 planeEnd)
{
	Vec2 planeAxis = planeEnd - planeStart;
	Vec2 planeNormal = planeAxis.GetNormalized().GetRotatedMinus90Degrees();

	float dotStart = DotProduct2D(capsule.m_start, planeNormal);
	float dotEnd = DotProduct2D(capsule.m_end, planeNormal);

	return dotStart * dotEnd < 0;
}

bool PushCapsuleOutOfPoint2D(Vec2 point, Capsule2& capsule)
{
	if (!IsPointInsideCapsule2D(point, capsule))
	{
		return false;
	}

	bool isPushed = false;
	if (IsPointInsideDisc2D(point, capsule.m_start, capsule.m_radius))
	{
		Vec2 pointToStart = capsule.m_start - point;
		float penDepth = capsule.m_radius - pointToStart.GetLength();
		capsule.m_start += (pointToStart.GetNormalized() * penDepth);
		isPushed |= true;
	}
	if (IsPointInsideDisc2D(point, capsule.m_end, capsule.m_radius))
	{
		Vec2 pointToEnd = capsule.m_end - point;
		float penDepth = capsule.m_radius - pointToEnd.GetLength();
		capsule.m_end += (pointToEnd.GetNormalized() * penDepth);
		isPushed |= true;
	}
	capsule.FixLength();

	return isPushed;
}

bool PushCapsuleOutOfPlane2D(Vec2 planeStart, Vec2 planeEnd, Capsule2& capsule)
{
	bool result = false;

	Vec2 nearestStart = GetNearestPointOnLineSegment2D(capsule.m_start, planeStart, planeEnd);
	result |= PushCapsuleOutOfPoint2D(nearestStart, capsule);

	Vec2 nearestEnd = GetNearestPointOnLineSegment2D(capsule.m_end, planeStart, planeEnd);
	result |= PushCapsuleOutOfPoint2D(nearestEnd, capsule);

	return result;
}

bool IsPointInsideAABB3D(Vec3 const& point, AABB3 const& box)
{
	return (point.x <= box.m_maxs.x && point.x >= box.m_mins.x)
		&& (point.y <= box.m_maxs.y && point.y >= box.m_mins.y)
		&& (point.z <= box.m_maxs.z && point.z >= box.m_mins.z);
}

bool IsPointInsideCapsule3D(Vec3 const& point, Capsule3 const& capsule)
{
	return capsule.IsPointInside(point);
}

bool IsPointInsideSphere3D(Vec3 const& point, Vec3 const& sphereCenter, float radius)
{
	Vec3 toPoint = point - sphereCenter;
	float distanceSquared = toPoint.GetLengthSquared();
	if (distanceSquared <= radius * radius)
	{
		return true;
	}
	return false;
}
bool IsPointInsideZCylinder3D(Vec3 const& point, Vec2 const& cylCenterXY, FloatRange cylMinMaxZ, float radius)
{
	if (point.z > cylMinMaxZ.m_max || point.z < cylMinMaxZ.m_min)
	{
		return false;
	}
	Vec2 pointXY = Vec2(point.x, point.y);
	Vec2 toPoint = pointXY - cylCenterXY;
	float distanceSquared = toPoint.GetLengthSquared();
	if (distanceSquared > radius * radius)
	{
		return false;
	}
	return true;
}
bool IsPointInsideAABB3D_Double(DoubleVec3 const& point, DoubleAABB3 const& box)
{
	return (point.x <= box.m_maxs.x && point.x >= box.m_mins.x)
		&& (point.y <= box.m_maxs.y && point.y >= box.m_mins.y)
		&& (point.z <= box.m_maxs.z && point.z >= box.m_mins.z);
}
bool IsPointInsideCapsule3D_Double(DoubleVec3 const& point, DoubleCapsule3 const& capsule)
{
	return capsule.IsPointInside(point);
}
bool IsPointInsideSphere3D_Double(DoubleVec3 const& point, DoubleVec3 const& sphereCenter, double radius)
{
	DoubleVec3 toPoint = point - sphereCenter;
	double distanceSquared = toPoint.GetLengthSquared();

	return distanceSquared < radius * radius;
}
//..............................
Vec2 GetNearestPointOnDisc2D(Vec2 const& referencePosition, Vec2 const& discCenter, float discRadius)
{
	Vec2 toPoint = referencePosition - discCenter;
	Vec2 point = discCenter + toPoint.GetNormalized() * discRadius;
	return point;
}
Vec2 GetNearestPointOnAABB2D(Vec2 const& referencePosition, AABB2 const& box)
{
	return box.GetNearestPoint(referencePosition);
}
Vec2 GetNearestPointOnInfiniteLine2D(Vec2 const& referencePosition, LineSegment2 const& infiniteLine)
{
	return GetNearestPointOnLineSegment2D(referencePosition, infiniteLine.m_start, infiniteLine.m_end);
}
Vec2 GetNearestPointOnInfiniteLine2D(Vec2 const& referencePosition, Vec2 const& pointOnLine, Vec2 const& anotherPointOnLine)
{
	Vec2 vecFromFirstPointTowardPoint = referencePosition - pointOnLine;
	Vec2 vecFromFirstPointToSecondPoint = anotherPointOnLine - pointOnLine;
	Vec2 vecFromFirstPointToNearest = GetProjectedOnto2D(vecFromFirstPointTowardPoint, vecFromFirstPointToSecondPoint);
	return pointOnLine + vecFromFirstPointToNearest;
}
Vec2 GetNearestPointOnLineSegment2D(Vec2 const& referencePosition, LineSegment2 const& lineSegment)
{
	return GetNearestPointOnLineSegment2D(referencePosition, lineSegment.m_start, lineSegment.m_end);
}
Vec2 GetNearestPointOnLineSegment2D(Vec2 const& referencePosition, Vec2 const& lineSegStart, Vec2 const& lineSegEnd)
{
	Vec2 vecFromEndTowardPoint = referencePosition - lineSegEnd;
	Vec2 vecFromStartTowardPoint = referencePosition - lineSegStart;
	Vec2 vecFromStartToEnd = lineSegEnd - lineSegStart;

	if (DotProduct2D(vecFromStartTowardPoint, vecFromStartToEnd) <= 0)
	{
		return lineSegStart;
	}
	if (DotProduct2D(vecFromEndTowardPoint, vecFromStartToEnd) >= 0)
	{
		return lineSegEnd;
	}
	Vec2 vecFromStartToNearest = GetProjectedOnto2D(vecFromStartTowardPoint, vecFromStartToEnd);
	return lineSegStart + vecFromStartToNearest;
}
Vec2 GetNearestPointOnCapsule2D(Vec2 const& referencePosition, Capsule2 const& capsule)
{
	return GetNearestPointOnCapsule2D(referencePosition, capsule.m_start, capsule.m_end, capsule.m_radius);
}
Vec2 GetNearestPointOnCapsule2D(Vec2 const& referencePosition, Vec2 const& boneStart, Vec2 const& boneEnd, float radius)
{
	Vec2 vecFromBoneEndTowardPoint = referencePosition - boneEnd;
	Vec2 vecFromBoneStartTowardPoint = referencePosition - boneStart;
	Vec2 vecFromBoneStartToBoneEnd = boneEnd - boneStart;

	if (DotProduct2D(vecFromBoneStartTowardPoint, vecFromBoneStartToBoneEnd) <= 0)
	{
		return GetNearestPointOnDisc2D(referencePosition, boneStart, radius);
	}
	if (DotProduct2D(vecFromBoneEndTowardPoint, vecFromBoneStartToBoneEnd) >= 0)
	{
		return GetNearestPointOnDisc2D(referencePosition, boneEnd, radius);
	}

	Vec2 obbCenter = (boneEnd + boneStart) * 0.5f;
	Vec2 obbIBasis = (boneEnd - boneStart).GetNormalized();
	Vec2 obbHalfDimension = Vec2((boneEnd - boneStart).GetLength() * 0.5f, radius);
	OBB2 body = OBB2(obbCenter, obbIBasis, obbHalfDimension);
	Vec2 a = GetNearestPointOnOBB2D(referencePosition, body);
	return GetNearestPointOnOBB2D(referencePosition, body);
}
Vec2 GetNearestPointOnOBB2D(Vec2 const& referencePosition, OBB2 const& orientedBox)
{
	return orientedBox.GetNearestPoint(referencePosition);
}
Vec3 GetNearestPointOnAABB3D(Vec3 const& referencePosition, AABB3 const& box)
{
	return box.GetNearestPoint(referencePosition);
}

Vec3 GetNearestPointOnCapsule3D(Vec3 const& referencePosition, Capsule3 const& capsule)
{
	return capsule.GetNearestPoint(referencePosition);

}

Vec3 GetNearestPointOnOBB3D(Vec3 const& referencePosition, OBB3 const& box)
{
	return box.GetNearestPoint(referencePosition);
}

Vec3 GetNearestPointOnSphere3D(Vec3 const& referencePosition, Vec3 const& center, float radius)
{
	Vec3 toPoint = referencePosition - center;
	float distance = toPoint.GetLength();
	if (distance > radius)
	{
		distance = radius;
	}
	return center + toPoint.GetNormalized() * distance;
}
Vec3 GetNearestPointOnZCynlinder3D(Vec3 const& referencePosition, Vec2 const& centerXY, FloatRange minMaxZ, float radius)
{
	Vec2 toPointXY = Vec2(referencePosition.x, referencePosition.y) - centerXY;
	float distance = toPointXY.GetLength();
	if (distance > radius)
	{
		distance = radius;
	}
	float zPos = Clamp(referencePosition.z, minMaxZ.m_min, minMaxZ.m_max);
	return Vec3(centerXY.x, centerXY.y, zPos) + Vec3(toPointXY.x, toPointXY.y, 0.f).GetNormalized() * distance;
}

Vec3 GetNearestPointOnLineSegment3D(Vec3 start, Vec3 end, Vec3 point)
{
	Vec3 startToEnd = end - start;
	float t = DotProduct3D(point - start, startToEnd) / DotProduct3D(startToEnd, startToEnd);
	return start + Clamp(t, 0.f, 1.f) * startToEnd;
}

Vec3 GetNearestPointOnLineSegment3D(LineSegment3 line, Vec3 point)
{
	return GetNearestPointOnLineSegment3D(line.m_start, line.m_end, point);
}

std::vector<Vec3> GetNearestPointsBetweenLines3D(LineSegment3 const& lineA, LineSegment3 const& lineB)
{
	std::vector<Vec3> nearestPoints;

	Vec3 axisA = lineA.GetAxis();
	Vec3 axisB = lineB.GetAxis();
	float dotAxisAB = DotProduct3D(axisA, axisB);

	if (dotAxisAB == 1 || dotAxisAB == -1)  // Lines are parallel or anti-parallel
	{
		nearestPoints.push_back(lineA.m_start);
		nearestPoints.push_back(lineB.m_start);
		return nearestPoints;
	}

	Vec3 v = lineA.m_start - lineB.m_start;
	float dA1 = DotProduct3D(axisA, axisA);		// Squared length of line A's direction vector
	float dA2 = DotProduct3D(axisA, v);			// Projection of v onto line A's direction
	float dB1 = DotProduct3D(axisB, axisB);		// Squared length of line B's direction vector
	float dB2 = DotProduct3D(axisB, v);			// Projection of v onto line B's direction
	float tA = 0.f, tB = 0.f;
	float denom = dA1 * dB1 - dotAxisAB * dotAxisAB; // denominator for parametric equation solution

	if (denom != 0)  // If lines aren't parallel
	{
		// This equation comes from minimizing the distance between points on both lines
		tA = Clamp((dotAxisAB * dB2 - dA2 * dB1) / denom, 0.0f, 1.0f);
	}

	tB = (dotAxisAB * tA + dB2) / dB1;

	if (tB < 0.0f)
	{
		tB = 0.0f;
		tA = Clamp(-dA2 / dA1, 0.0f, 1.0f);
	}
	else if (tB > 1.0f)
	{
		tB = 1.0f;
		tA = Clamp((dotAxisAB - dA2) / dA1, 0.0f, 1.0f);
	}

	nearestPoints.push_back(lineA.m_start + tA * axisA);
	nearestPoints.push_back(lineB.m_start + tB * axisB);

	return nearestPoints;

}
DoubleVec3 GetNearestPointOnAABB3D_Double(DoubleVec3 const& referencePosition, DoubleAABB3 const& box)
{
	return box.GetNearestPoint(referencePosition);
}

DoubleVec3 GetNearestPointOnCapsule3D_Double(DoubleVec3 const& referencePosition, DoubleCapsule3 const& capsule)
{
	return capsule.GetNearestPoint(referencePosition);
}

DoubleVec3 GetNearestPointOnOBB3D_Double(DoubleVec3 const& referencePosition, DoubleOBB3 const& box)
{
	return box.GetNearestPoint(referencePosition);
}

DoubleVec3 GetNearestPointOnSphere3D_Double(DoubleVec3 const& referencePosition, DoubleVec3 const& center, double radius)
{
	DoubleVec3 toPoint = referencePosition - center;
	double distance = toPoint.GetLength();
	if (distance > radius)
	{
		distance = radius;
	}
	return center + toPoint.GetNormalized() * distance;
}

DoubleVec3 GetNearestPointOnLineSegment3D_Double(DoubleVec3 start, DoubleVec3 end, DoubleVec3 point)
{
	DoubleVec3 startToEnd = end - start;
	double t = DotProduct3D_Double(point - start, startToEnd) / DotProduct3D_Double(startToEnd, startToEnd);
	return start + Clamp_Double(t, 0.0, 1.0) * startToEnd;
}

DoubleVec3 GetNearestPointOnLineSegment3D_Double(DoubleLineSegment3 line, DoubleVec3 point)
{
	return GetNearestPointOnLineSegment3D_Double(line.m_start, line.m_end, point);
}

std::vector<DoubleVec3> GetNearestPointsBetweenLines3D_Double(DoubleLineSegment3 const& lineA, DoubleLineSegment3 const& lineB)
{
	std::vector<DoubleVec3> nearestPoints;

	DoubleVec3 axisA = lineA.GetAxis();
	DoubleVec3 axisB = lineB.GetAxis();
	double dotAxisAB = DotProduct3D_Double(axisA, axisB);

	if (dotAxisAB == 1 || dotAxisAB == -1)
	{
		//Same
		nearestPoints.push_back(lineA.m_start);
		nearestPoints.push_back(lineB.m_start);
		return nearestPoints;
	}

	DoubleVec3 v = lineA.m_start - lineB.m_start;
	double dA1 = DotProduct3D_Double(axisA, axisA);
	double dA2 = DotProduct3D_Double(axisA, v);
	double dB1 = DotProduct3D_Double(axisB, axisB);
	double dB2 = DotProduct3D_Double(axisB, v);
	double tA = 0, tB = 0;
	double denom = dA1 * dB1 - dotAxisAB * dotAxisAB;

	if (denom != 0)
	{
		tA = Clamp_Double((dotAxisAB * dB2 - dA2 * dB1) / denom, 0.0, 1.0);
	}

	tB = (dotAxisAB * tA + dB2) / dB1;

	if (tB < 0.0f)
	{
		tB = 0.0f;
		tA = Clamp_Double(-dA2 / dA1, 0.0f, 1.0f);
	}
	else if (tB > 1.0f)
	{
		tB = 1.0f;
		tA = Clamp_Double((dotAxisAB - dA2) / dA1, 0.0f, 1.0f);
	}

	nearestPoints.push_back(lineA.m_start + tA * axisA);
	nearestPoints.push_back(lineB.m_start + tB * axisB);

	return nearestPoints;
}

void Optimized_GetNearestPointsBetweenLines3D_Double(DoubleVec3* out_points, DoubleLineSegment3 const& lineA, DoubleLineSegment3 const& lineB)
{
	std::vector<DoubleVec3> nearestPoints;

	DoubleVec3 axisA = lineA.GetAxis();
	DoubleVec3 axisB = lineB.GetAxis();
	double dotAxisAB = DotProduct3D_Double(axisA, axisB);

	if (dotAxisAB == 1 || dotAxisAB == -1)
	{
		//Same
		out_points[0] = lineA.m_start;
		out_points[1] = lineB.m_start;
		return;
	}

	DoubleVec3 v = lineA.m_start - lineB.m_start;
	double dA1 = DotProduct3D_Double(axisA, axisA);
	double dA2 = DotProduct3D_Double(axisA, v);
	double dB1 = DotProduct3D_Double(axisB, axisB);
	double dB2 = DotProduct3D_Double(axisB, v);
	double tA = 0, tB = 0;
	double denom = dA1 * dB1 - dotAxisAB * dotAxisAB;

	if (denom != 0)
	{
		tA = Clamp_Double((dotAxisAB * dB2 - dA2 * dB1) / denom, 0.0, 1.0);
	}

	tB = (dotAxisAB * tA + dB2) / dB1;

	if (tB < 0.0f)
	{
		tB = 0.0f;
		tA = Clamp_Double(-dA2 / dA1, 0.0f, 1.0f);
	}
	else if (tB > 1.0f)
	{
		tB = 1.0f;
		tA = Clamp_Double((dotAxisAB - dA2) / dA1, 0.0f, 1.0f);
	}

	out_points[0] = lineA.m_start + tA * axisA;
	out_points[1] = lineB.m_start + tB * axisB;
}

//..............................
bool PushDiscOutOfPoint2D(Vec2& mobileDiscCenter, float discRadius, Vec2 const& fixedPoint)
{
	if (!IsPointInsideDisc2D(mobileDiscCenter, fixedPoint, discRadius))
	{
		return false;
	}
	Vec2 toMobileDiscCenterVector = mobileDiscCenter - fixedPoint;
	float overlapDepth = discRadius - toMobileDiscCenterVector.GetLength();
	toMobileDiscCenterVector.SetLength(overlapDepth);
	mobileDiscCenter += toMobileDiscCenterVector;
	return true;
}
//..............................
bool PushDiscOutOfDisc2D(Vec2& mobileDiscCenter, float mobileDiscRadius, Vec2 const& fixedDiscCenter, float fixedDiscRadius)
{
	if (!DoDiscsOverlap2D(mobileDiscCenter, mobileDiscRadius, fixedDiscCenter, fixedDiscRadius))
	{
		return false;
	}
	Vec2 toMobileDiscCenter = mobileDiscCenter - fixedDiscCenter;
	float overlapDepth = (mobileDiscRadius + fixedDiscRadius) - fabsf(toMobileDiscCenter.GetLength());
	toMobileDiscCenter.SetLength(overlapDepth);
	mobileDiscCenter += toMobileDiscCenter;
	return true;
}
//..............................
bool PushDiscsOutOfEachOther2D(Vec2& aCenter, float aRadius, Vec2& bCenter, float bRadius)
{
	if (!DoDiscsOverlap2D(aCenter, aRadius, bCenter, bRadius))
	{
		return false;
	}
	Vec2 distanceVec = bCenter - aCenter;
	float overlapDepth = ((aRadius + bRadius) - fabsf(distanceVec.GetLength())) * 0.5f;
	distanceVec.SetLength(overlapDepth);
	aCenter -= distanceVec;
	bCenter += distanceVec;
	return true;
}
//..............................
bool PushDiscOutOfAABB2D(Vec2& mobileDiscCenter, float discRadius, AABB2 const& fixedBox)
{
	Vec2 nearestPoint = fixedBox.GetNearestPoint(mobileDiscCenter);
	return PushDiscOutOfPoint2D(mobileDiscCenter, discRadius, nearestPoint);
}

bool PushZCylinderOutOfPoint3D(Vec2& mobileCylXYCenter, FloatRange& cylMinMaxZ, float cylRadius, Vec3 const& fixedPoint)
{
	if (!IsPointInsideZCylinder3D(fixedPoint, mobileCylXYCenter, cylMinMaxZ, cylRadius))
	{
		return false;
	}
	float minMaxDiff = cylMinMaxZ.m_max - cylMinMaxZ.m_min;

	Vec2 toMobileDiscCenterVector = mobileCylXYCenter - Vec2(fixedPoint.x, fixedPoint.y);
	float overlapDepth = cylRadius - toMobileDiscCenterVector.GetLength();
	toMobileDiscCenterVector.SetLength(overlapDepth);
	mobileCylXYCenter += toMobileDiscCenterVector;

	if (IsPointInsideZCylinder3D(fixedPoint, mobileCylXYCenter, cylMinMaxZ, cylRadius))
	{
		if (cylMinMaxZ.m_min < fixedPoint.z && cylMinMaxZ.GetMedian() > fixedPoint.z)
		{
			cylMinMaxZ.m_min += (fixedPoint.z - cylMinMaxZ.m_min);
			cylMinMaxZ.m_max = cylMinMaxZ.m_min + minMaxDiff;
		}
		else if (cylMinMaxZ.m_max > fixedPoint.z && cylMinMaxZ.GetMedian() < fixedPoint.z)
		{
			cylMinMaxZ.m_max -= (cylMinMaxZ.m_max - fixedPoint.z);
			cylMinMaxZ.m_min = cylMinMaxZ.m_max - minMaxDiff;
		}
	}

	return true;
}

bool PushZCylinderOutOfSphere3D(Vec2& mobileCylXYCenter, FloatRange& cylMinMaxZ, float cylRadius, Vec3 const& fixedDiscCenter, float fixedDiscRadius)
{
	if (!DoZCylinderAndSphereOverlap3D(mobileCylXYCenter, cylRadius, cylMinMaxZ, fixedDiscCenter, fixedDiscRadius))
	{
		return false;
	}
	Vec2 toMobileDiscCenter = mobileCylXYCenter - Vec2(fixedDiscCenter.x, fixedDiscCenter.y);
	float overlapDepth = (cylRadius + fixedDiscRadius) - fabsf(toMobileDiscCenter.GetLength());
	toMobileDiscCenter.SetLength(overlapDepth);
	mobileCylXYCenter += toMobileDiscCenter;
	return true;
}

bool PushZCylindersOutOfZCylinders3D(Vec2& mobileCylCenter, FloatRange& mobileCylMinMaxZ, float mobileCylRadius, Vec2 const& fixedCylCenter, FloatRange const& fixedCylMinMaxZ, float fixedCylRadius)
{
	if (!DoZCylindersOverlap3D(mobileCylCenter, mobileCylRadius, mobileCylMinMaxZ, fixedCylCenter, fixedCylRadius, fixedCylMinMaxZ))
	{
		return false;
	}
	Vec2 toMobileDiscCenter = mobileCylCenter - Vec2(fixedCylCenter.x, fixedCylCenter.y);
	float overlapDepth = (mobileCylRadius + fixedCylRadius) - fabsf(toMobileDiscCenter.GetLength());
	toMobileDiscCenter.SetLength(overlapDepth);
	mobileCylCenter += toMobileDiscCenter;
	return true;
}

bool PushZCylindersOutOfEachOther3D(Vec2& cylACenter, FloatRange& cylAMinMaxZ, float cylARadius, Vec2& cylBCenter, FloatRange& cylBMinMaxZ, float cylBRadius)
{
	if (!DoZCylindersOverlap3D(cylACenter, cylARadius, cylAMinMaxZ, cylBCenter, cylBRadius, cylBMinMaxZ))
	{
		return false;
	}
	Vec2 distanceVec = cylBCenter - cylACenter;
	if (distanceVec.GetLengthSquared() <= 0)
	{
		cylACenter -= Vec2(cylBRadius, cylBRadius);
		cylBCenter += Vec2(-cylARadius, -cylARadius);
		return true;
	}
	float overlapDepth = ((cylARadius + cylBRadius) - fabsf(distanceVec.GetLength())) * 0.5f;
	distanceVec.SetLength(overlapDepth);
	cylACenter -= distanceVec;
	cylBCenter += distanceVec;
	return true;
}

bool PushZCylinderOutOfAABB3D(Vec2& mobileCylXYCenter, FloatRange& cylMinMaxZ, float cylRadius, AABB3 const& fixedBox)
{
	if (!DoZCylinderAndAABBOverlap3D(mobileCylXYCenter, cylRadius, cylMinMaxZ, fixedBox))
	{
		return false;
	}
	Vec3 nearestPoint = GetNearestPointOnAABB3D(Vec3(mobileCylXYCenter.x, mobileCylXYCenter.y, (cylMinMaxZ.m_max + cylMinMaxZ.m_min) * 0.5f), fixedBox);
	return PushZCylinderOutOfPoint3D(mobileCylXYCenter, cylMinMaxZ, cylRadius, nearestPoint);
}

bool PushSphereOutOfSphere3D(Vec3& posA, float radiusA, Vec3& posB, float radiusB, bool isBStatic)
{
	if (!DoSpheresOverlap3D(posA, radiusA, posB, radiusB))
	{
		return false;
	}
	if (isBStatic)
	{
		Vec3 dist = posA - posB;
		float overlapLength = (radiusA + radiusB) - fabsf(dist.GetLength());
		dist.SetLength(overlapLength);
		posA += dist;
		return true;
	}
	else
	{
		Vec3 dist = posB - posA;
		float overlapDepth = (radiusA + radiusB) - fabsf(dist.GetLength());
		dist.SetLength(overlapDepth * 0.5f);
		posA -= dist;
		posB += dist;
		return true;
	}
}

bool PushSphereOutOfZCylinder3D(Vec3& pos, float radius, Vec2 const& cylCenter, FloatRange const& cylMinMaxZ, float cylRadius)
{
	if (!DoZCylinderAndSphereOverlap3D(cylCenter, cylRadius, cylMinMaxZ, pos, radius))
	{
		return false;
	}
	float currentZ = Clamp(pos.z, cylMinMaxZ.m_min, cylMinMaxZ.m_max);
	Vec3 toSphereCenter = pos - Vec3(cylCenter.x, cylCenter.y, currentZ);
	float overlapDepth = (radius + cylRadius) - fabsf(toSphereCenter.GetLength());
	toSphereCenter.SetLength(overlapDepth);
	pos += toSphereCenter;
	return true;
}

bool PushSphereOutOfPlane3D(Vec3& center, float radius, Plane3 const& plane)
{
	if (!DoSphereAndPlaneOverlap3D(center, radius, plane))
	{
		return false;
	}
	Vec3 toSphereCenter = (center - plane.GetNearestPoint(center)).GetNormalized();
	if (toSphereCenter == Vec3::ZERO)
	{
		center += plane.m_normal * radius;
	}
	else
	{
		center += toSphereCenter.GetNormalized() * (radius - plane.GetAltitudeOfPoint(center));
	}

	return true;
}

bool PushSphereOutOfAABB3(Vec3& center, float radius, AABB3 const& box)
{
	if (!DoSphereAndAABBOverlap3D(center, radius, box))
	{
		return false;
	}
	Vec3 toSphereCenter = (center - box.GetNearestPoint(center));
	Vec3 penDir = toSphereCenter.GetNormalized() * (radius - toSphereCenter.GetLength());
	center += penDir;
	return true;
}

bool PushSphereOutOfOBB3(Vec3& center, float radius, OBB3 const& obb)
{
	Vec3 kBasisNormal = CrossProduct3D(obb.m_iBasisNormal, obb.m_jBasisNormal);

	Mat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	Mat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	AABB3 aabb;
	aabb.m_mins = Vec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = Vec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	Vec3 localSphereCeneter = obbWtoL.TransformPosition3D(center);

	if (PushSphereOutOfAABB3(localSphereCeneter, radius, aabb))
	{
		center = obbLtoW.TransformPosition3D(localSphereCeneter);
		return true;
	}

	return false;
}

bool PushSphereOutOfCapsule3(Vec3& center, float radius, Capsule3& capsule, bool isCapsuleStatic)
{
	Vec3 nearestPointToBone = GetNearestPointOnLineSegment3D(capsule.m_start, capsule.m_end, center);
	Vec3 fromSphereCenterToBone = nearestPointToBone - center;
	float distanceFromSphereToBoneSquared = fromSphereCenterToBone.GetLengthSquared();
	if (distanceFromSphereToBoneSquared < (capsule.m_radius + radius) * (capsule.m_radius + radius))
	{
		float penetrationLength = (capsule.m_radius + radius) - sqrtf(distanceFromSphereToBoneSquared);
		Vec3 pushedNearestPointToBone = nearestPointToBone + fromSphereCenterToBone.GetNormalized() * penetrationLength;
		Vec3 displacement = pushedNearestPointToBone - nearestPointToBone;

		if (capsule.m_start == nearestPointToBone)
		{
			if (isCapsuleStatic)
			{
				center += displacement;
			}
			else
			{
				center += displacement * 0.5f;
				capsule.m_start -= displacement * 0.5f;
			}
		}
		else if (capsule.m_end == nearestPointToBone)
		{
			if (isCapsuleStatic)
			{
				center += displacement;
			}
			else
			{
				center += displacement * 0.5f;
				capsule.m_end -= displacement * 0.5f;
			}
		}
		else
		{
			if (isCapsuleStatic)
			{
				// flip end start ratio to make it look more real
				center += displacement;
			}
			else
			{
				float axisLength = capsule.GetAxisLength();
				Vec3 startToNearestPoint = nearestPointToBone - capsule.m_start;
				float startToNearestPointLength = startToNearestPoint.GetLength();

				float pushStartRatio = startToNearestPointLength / axisLength;
				float pushEndRatio = (axisLength - startToNearestPointLength) / axisLength;

				center += displacement * 0.5f;
				capsule.m_start -= displacement * 0.5f * pushEndRatio;
				capsule.m_end -= displacement * 0.5f * pushStartRatio;
			}
		}
	}

	return false;
}

bool PushSphereOutOfPoint3D(Vec3& center, float radius, Vec3 const& point)
{
	if (!IsPointInsideSphere3D(point, center, radius))
	{
		return false;
	}
	Vec3 toSphereCenter = center - point;
	toSphereCenter.SetLength(radius - toSphereCenter.GetLength());
	center += toSphereCenter;
	return true;
}

bool PushCapsuleOutOfPoint3D(Capsule3& capsule, Vec3 const& point, bool fixLength)
{
	if (!IsPointInsideCapsule3D(point, capsule))
	{
		return false;
	}

	bool isPushed = false;
	if (IsPointInsideSphere3D(point, capsule.m_start, capsule.m_radius))
	{
		Vec3 pointToStart = capsule.m_start - point;
		float penDepth = capsule.m_radius - pointToStart.GetLength();
		capsule.m_start += (pointToStart.GetNormalized() * penDepth);
		isPushed |= true;
	}
	if (IsPointInsideSphere3D(point, capsule.m_end, capsule.m_radius))
	{
		Vec3 pointToEnd = capsule.m_end - point;
		float penDepth = capsule.m_radius - pointToEnd.GetLength();
		capsule.m_end += (pointToEnd.GetNormalized() * penDepth);
		isPushed |= true;
	}
	if (isPushed && fixLength)
	{
		capsule.FixLength();
	}

	return isPushed;
}

bool PushCapsuleOutOfPlane3D(Capsule3& capsule, Plane3 const& plane, bool fixLength)
{
	bool result = false;

	Vec3 Q = plane.m_normal * plane.m_distanceFromOrigin;
	float dotEnd = DotProduct3D(Q - capsule.m_end, plane.m_normal);
	float dotStart = DotProduct3D(Q - capsule.m_start, plane.m_normal);

	if (dotEnd * dotStart <= 0.f)
	{
		result = true;
		float alt = 0.f;
		float alt1 = 0.f;
		float alt2 = 0.f;
		if (dotEnd >= 0)
		{
			alt1 = plane.GetAltitudeOfPoint(capsule.m_end);
			alt1 = fabsf(alt1);
		}
		if (dotStart >= 0)
		{
			alt2 = plane.GetAltitudeOfPoint(capsule.m_start);
			alt2 = fabsf(alt2);
		}
		alt = (alt1 > alt2) ? alt1 : alt2;

		capsule.m_end += plane.m_normal * alt;
		capsule.m_start += plane.m_normal * alt;
	}

	Vec3 nearestStart = plane.GetNearestPoint(capsule.m_start);
	result |= PushCapsuleOutOfPoint3D(capsule, nearestStart, fixLength);

	Vec3 nearestEnd = plane.GetNearestPoint(capsule.m_end);
	result |= PushCapsuleOutOfPoint3D(capsule, nearestEnd, fixLength);

	return result;
}

bool PushCapsuleOutOfAABB3D(Capsule3& capsule, AABB3 const& aabb, bool fixLength)
{
	if (DoSphereAndAABBOverlap3D(capsule.m_start, capsule.m_radius, aabb))
	{
		Vec3 toSphereCenter = (capsule.m_start - aabb.GetNearestPoint(capsule.m_start));
		float toSphereDist = toSphereCenter.GetLength();
		Vec3 penDir = toSphereCenter.GetNormalized() * (capsule.m_radius - toSphereDist);
		float axisLength = capsule.GetAxisLength();
		float pushStartRatio = toSphereDist / axisLength;
		float pushEndRatio = (axisLength - toSphereDist) / axisLength;

		capsule.m_start += penDir * pushEndRatio;
		capsule.m_end += penDir * pushStartRatio;
		if (fixLength)
		{
			capsule.FixLength();
		}
		return true;
	}

	if (DoSphereAndAABBOverlap3D(capsule.m_end, capsule.m_radius, aabb))
	{
		Vec3 toSphereCenter = (capsule.m_end - aabb.GetNearestPoint(capsule.m_end));
		float toSphereDist = toSphereCenter.GetLength();
		Vec3 penDir = toSphereCenter.GetNormalized() * (capsule.m_radius - toSphereDist);
		float axisLength = capsule.GetAxisLength();
		float pushStartRatio = toSphereDist / axisLength;
		float pushEndRatio = (axisLength - toSphereDist) / axisLength;

		capsule.m_start += penDir * pushStartRatio;
		capsule.m_end += penDir * pushEndRatio;
		if (fixLength)
		{
			capsule.FixLength();
		}
		return true;
	}

	// Best Possible Intersects
	float tValues[6];
	float bestTValue = FLT_MAX;
	float smallestDistanceSquared = FLT_MAX;
	Vec3 bestIntersectPoint = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vec3 bestNearestPointAABB = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	Vec3 axis = capsule.GetAxis();
	float rayZScale = 1.0f / axis.z;
	float rayYScale = 1.0f / axis.y;
	float rayXScale = 1.0f / axis.x;
	tValues[0] = ((aabb.m_maxs.z - capsule.m_start.z) * rayZScale);
	tValues[1] = ((aabb.m_mins.z - capsule.m_start.z) * rayZScale);
	tValues[2] = ((aabb.m_maxs.y - capsule.m_start.y) * rayYScale);
	tValues[3] = ((aabb.m_mins.y - capsule.m_start.y) * rayYScale);
	tValues[4] = ((aabb.m_maxs.x - capsule.m_start.x) * rayXScale);
	tValues[5] = ((aabb.m_mins.x - capsule.m_start.x) * rayXScale);

	for (float tValue : tValues)
	{
		if (tValue >= 0.0f && tValue <= 1.0f)
		{
			Vec3 intersectPoint = capsule.m_start + (tValue * axis);
			Vec3 nearestPointAABB = aabb.GetNearestEdgePosition(intersectPoint);
			float distanceSquared = GetDistanceSquared3D(intersectPoint, nearestPointAABB);
			if (distanceSquared < smallestDistanceSquared)
			{
				smallestDistanceSquared = distanceSquared;
				bestIntersectPoint = intersectPoint;
				bestNearestPointAABB = nearestPointAABB;
				bestTValue = tValue;
			}
		}
	}

	// Doesn't hit
	if (bestTValue < -1.0f || bestTValue > 2.0f)
	{
		return false;
	}

	Vec3 nearestPointBone = GetNearestPointOnLineSegment3D(capsule.m_start, capsule.m_end, bestNearestPointAABB);
	bestNearestPointAABB = aabb.GetNearestEdgePosition(nearestPointBone);

	if (IsPointInsideAABB3D(nearestPointBone, aabb))
	{
		Vec3 center = aabb.GetCenter();
		Vec3 displacementCenterToBone = nearestPointBone - center;
		Vec3 displacementCenterToCorner = bestNearestPointAABB - center;

		if (displacementCenterToBone.GetLengthSquared() < displacementCenterToCorner.GetLengthSquared())
		{
			Vec3 displacement = bestNearestPointAABB - nearestPointBone;
			displacement.SetLength(displacement.GetLength() + capsule.m_radius);
			capsule.m_start += displacement;
			capsule.m_end += displacement;
			if (fixLength)
			{
				capsule.FixLength();
			}
			return true;
		}
	}
	else
	{
		Vec3 displacement = nearestPointBone - bestNearestPointAABB;
		float distanceSquared = displacement.GetLengthSquared();
		if (distanceSquared < capsule.m_radius * capsule.m_radius)
		{
			float offset = capsule.m_radius - sqrtf(distanceSquared);
			displacement.SetLength(offset);
			capsule.m_start += displacement;
			capsule.m_end += displacement;
			if (fixLength)
			{
				capsule.FixLength();
			}
			return true;
		}
	}

	return false;
}

bool PushCapsuleOutOfOBB3D(Capsule3& capsule, OBB3 const& obb, bool fixLength /*= true*/)
{
	Vec3 kBasisNormal = CrossProduct3D(obb.m_iBasisNormal, obb.m_jBasisNormal);

	Mat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	Mat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	AABB3 aabb;
	aabb.m_mins = Vec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = Vec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	Capsule3 localCapsule;
	localCapsule.m_radius = capsule.m_radius;
	localCapsule.m_start = obbWtoL.TransformPosition3D(capsule.m_start);
	localCapsule.m_end = obbWtoL.TransformPosition3D(capsule.m_end);

	if (PushCapsuleOutOfAABB3D(localCapsule, aabb, false))
	{
		capsule.m_start = obbLtoW.TransformPosition3D(localCapsule.m_start);
		capsule.m_end = obbLtoW.TransformPosition3D(localCapsule.m_end);
		if (fixLength)
		{
			capsule.FixLength();
		}
		return true;
	}

	return false;
}

bool PushCapsuleOutOfCapsule3D(Capsule3& capsuleA, Capsule3& capsuleB, bool isBStatic)
{
	std::vector<Vec3> nearestPointsOn2Bones = GetNearestPointsBetweenLines3D(capsuleA.GetBone(), capsuleB.GetBone());

	Vec3 dist2Points = nearestPointsOn2Bones[0] - nearestPointsOn2Bones[1];
	float dist2PointsLengthSquared = dist2Points.GetLengthSquared();

	if (dist2PointsLengthSquared < (capsuleA.m_radius + capsuleB.m_radius) * (capsuleA.m_radius + capsuleB.m_radius))
	{
		float penDepth = capsuleA.m_radius + capsuleB.m_radius - sqrtf(dist2PointsLengthSquared);
		Vec3 pushDir = dist2Points * penDepth;

		if (isBStatic)
		{
			float axisLength = capsuleA.GetAxisLength();
			Vec3 startToNearestPoint = nearestPointsOn2Bones[0] - capsuleA.m_start;
			float startToNearestPointLength = startToNearestPoint.GetLength();

			float pushStartRatio = startToNearestPointLength / axisLength;
			float pushEndRatio = (axisLength - startToNearestPointLength) / axisLength;

			capsuleA.m_start += pushDir * pushEndRatio;
			capsuleA.m_end += pushDir * pushStartRatio;
		}
		else
		{
			pushDir *= 0.5f;

			float axisALength = capsuleA.GetAxisLength();
			Vec3 startAToNearestPoint = nearestPointsOn2Bones[0] - capsuleA.m_start;
			float startAToNearestPointLength = startAToNearestPoint.GetLength();

			float pushAStartRatio = startAToNearestPointLength / axisALength;
			float pushAEndRatio = (axisALength - startAToNearestPointLength) / axisALength;

			capsuleA.m_start += pushDir * pushAEndRatio;
			capsuleA.m_end += pushDir * pushAStartRatio;


			float axisBLength = capsuleB.GetAxisLength();
			Vec3 startBToNearestPoint = nearestPointsOn2Bones[1] - capsuleB.m_start;
			float startBToNearestPointLength = startBToNearestPoint.GetLength();

			float pushBStartRatio = startBToNearestPointLength / axisBLength;
			float pushBEndRatio = (axisBLength - startBToNearestPointLength) / axisBLength;

			capsuleB.m_start -= pushDir * pushBEndRatio;
			capsuleB.m_end -= pushDir * pushBStartRatio;
		}
		return true;
	}
	return false;
}

bool PushCapsuleOutOfSphere3D(Capsule3& capsule, Vec3& sCenter, float sRadius, bool isSphereStatic /*= true*/)
{
	Vec3 nearestPointToBone = GetNearestPointOnLineSegment3D(capsule.m_start, capsule.m_end, sCenter);
	Vec3 fromSphereCenterToBone = nearestPointToBone - sCenter;
	float distanceFromSphereToBoneSquared = fromSphereCenterToBone.GetLengthSquared();
	if (distanceFromSphereToBoneSquared < (capsule.m_radius + sRadius) * (capsule.m_radius + sRadius))
	{
		float penetrationLength = (capsule.m_radius + sRadius) - sqrtf(distanceFromSphereToBoneSquared);
		Vec3 pushedNearestPointToBone = nearestPointToBone + fromSphereCenterToBone.GetNormalized() * penetrationLength;
		Vec3 displacement = pushedNearestPointToBone - nearestPointToBone;

		if (capsule.m_start == nearestPointToBone)
		{
			if (isSphereStatic)
			{
				capsule.m_start += displacement;
			}
			else
			{
				capsule.m_start += displacement * 0.5f;
				sCenter -= displacement * 0.5f;
			}
		}
		else if (capsule.m_end == nearestPointToBone)
		{
			if (isSphereStatic)
			{
				capsule.m_end += displacement;
			}
			else
			{
				capsule.m_end += displacement * 0.5f;
				sCenter -= displacement * 0.5f;
			}
		}
		else
		{
			float axisLength = capsule.GetAxisLength();
			Vec3 startToNearestPoint = nearestPointToBone - capsule.m_start;
			float startToNearestPointLength = startToNearestPoint.GetLength();

			float pushStartRatio = startToNearestPointLength / axisLength;
			float pushEndRatio = (axisLength - startToNearestPointLength) / axisLength;

			if (isSphereStatic)
			{
				// flip end start ratio to make it look more real
				capsule.m_start += displacement * pushEndRatio;
				capsule.m_end += displacement * pushStartRatio;
			}
			else
			{
				capsule.m_start += displacement * 0.5f * pushEndRatio;
				capsule.m_end += displacement * 0.5f * pushStartRatio;
				sCenter -= displacement * 0.5f;
			}
		}
		return true;
	}

	return false;
}

bool PushSphereOutOfSphere3D_Double(DoubleVec3& posA, double radiusA, DoubleVec3& posB, double radiusB, bool isBStatic)
{
	if (!DoSpheresOverlap3D_Double(posA, radiusA, posB, radiusB))
	{
		return false;
	}

	if (isBStatic)
	{
		DoubleVec3 dist = posA - posB;
		double overlapLength = (radiusA + radiusB) - fabs(dist.GetLength());
		dist.SetLength(overlapLength);
		posA += dist;
		return true;
	}

	DoubleVec3 dist = posB - posA;
	double overlapDepth = (radiusA + radiusB) - fabs(dist.GetLength());
	dist.SetLength(overlapDepth);
	posA -= dist * 0.5;
	posB += dist * 0.5;
	return true;
}

bool PushSphereOutOfPlane3D_Double(DoubleVec3& center, double radius, DoublePlane3 const& plane)
{
	if (!DoSphereAndPlaneOverlap3D_Double(center, radius, plane))
	{
		return false;
	}
	DoubleVec3 nearestPoint(plane.GetNearestPoint(center));
	DoubleVec3 toSphereCenter = (center - nearestPoint).GetNormalized();
	if (toSphereCenter == DoubleVec3::ZERO)
	{
		center += plane.m_normal * radius;
	}
	else
	{
		center += toSphereCenter.GetNormalized() * (radius - plane.GetAltitudeOfPoint(center));
	}

	return true;
}

bool PushSphereOutOfAABB3_Double(DoubleVec3& center, double radius, DoubleAABB3 const& box)
{
	if (!DoSphereAndAABBOverlap3D_Double(center, radius, box))
	{
		return false;
	}
	DoubleVec3 toSphereCenter = (center - box.GetNearestPoint(center));
	DoubleVec3 penDir = toSphereCenter.GetNormalized() * (radius - toSphereCenter.GetLength());
	center += penDir;
	return true;
}

bool PushSphereOutOfOBB3_Double(DoubleVec3& center, double radius, DoubleOBB3 const& obb)
{
	DoubleVec3 kBasisNormal = CrossProduct3D(obb.m_iBasisNormal, obb.m_jBasisNormal);

	DoubleMat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	DoubleMat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	DoubleAABB3 aabb;
	aabb.m_mins = DoubleVec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = DoubleVec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	DoubleVec3 localSphereCeneter = obbWtoL.TransformPosition3D(center);

	if (PushSphereOutOfAABB3_Double(localSphereCeneter, radius, aabb))
	{
		center = obbLtoW.TransformPosition3D(localSphereCeneter);
		return true;
	}

	return false;
}

bool PushSphereOutOfCapsule3_Double(DoubleVec3& center, double radius, DoubleCapsule3& capsule, bool isCapsuleStatic)
{
	DoubleVec3 nearestPointToBone = GetNearestPointOnLineSegment3D_Double(capsule.m_start, capsule.m_end, center);
	DoubleVec3 fromSphereCenterToBone = nearestPointToBone - center;
	double distanceFromSphereToBoneSquared = fromSphereCenterToBone.GetLengthSquared();
	if (distanceFromSphereToBoneSquared < (capsule.m_radius + radius) * (capsule.m_radius + radius))
	{
		double penetrationLength = (capsule.m_radius + radius) - sqrt(distanceFromSphereToBoneSquared);
		DoubleVec3 pushedNearestPointToBone = nearestPointToBone + fromSphereCenterToBone.GetNormalized() * penetrationLength;
		DoubleVec3 displacement = pushedNearestPointToBone - nearestPointToBone;

		if (capsule.m_start == nearestPointToBone)
		{
			if (isCapsuleStatic)
			{
				center -= displacement;
			}
			else
			{
				center -= displacement * 0.5;
				capsule.m_start += displacement * 0.5;
			}
		}
		else if (capsule.m_end == nearestPointToBone)
		{
			if (isCapsuleStatic)
			{
				center -= displacement;
			}
			else
			{
				center -= displacement * 0.5;
				capsule.m_end += displacement * 0.5;
			}
		}
		else
		{
			if (isCapsuleStatic)
			{
				// flip end start ratio to make it look more real
				center -= displacement;
			}
			else
			{
				double axisLength = capsule.GetAxisLength();
				DoubleVec3 startToNearestPoint = nearestPointToBone - capsule.m_start;
				double startToNearestPointLength = startToNearestPoint.GetLength();

				double pushStartRatio = startToNearestPointLength / axisLength;
				double pushEndRatio = (axisLength - startToNearestPointLength) / axisLength;

				center -= displacement * 0.5f;
				capsule.m_start += displacement * 0.5f * pushEndRatio;
				capsule.m_end += displacement * 0.5f * pushStartRatio;
			}
		}
		return true;
	}

	return false;
}

bool PushSphereOutOfPoint3D_Double(DoubleVec3& center, double radius, DoubleVec3 const& point)
{
	if (!IsPointInsideSphere3D_Double(point, center, radius))
	{
		return false;
	}
	DoubleVec3 toSphereCenter = center - point;
	toSphereCenter.SetLength(radius - toSphereCenter.GetLength());
	center += toSphereCenter;
	return true;
}

bool PushCapsuleOutOfPoint3D_Double(DoubleCapsule3& capsule, DoubleVec3 const& point, bool fixLength)
{
	DoubleVec3 pointOnBone = GetNearestPointOnLineSegment3D_Double(capsule.GetBone(), point);

	if (IsPointInsideSphere3D_Double(point, pointOnBone, capsule.m_radius))
	{
		DoubleVec3 pushDir = (pointOnBone - pointOnBone).GetNormalized();

		double axisALength = capsule.GetAxisLength();
		DoubleVec3 startAToNearestPoint = pointOnBone - capsule.m_start;
		double startAToNearestPointLength = startAToNearestPoint.GetLength();

		double pushAStartRatio = startAToNearestPointLength / axisALength;
		double pushAEndRatio = (axisALength - startAToNearestPointLength) / axisALength;

		capsule.m_start += pushDir * pushAEndRatio;
		capsule.m_end += pushDir * pushAStartRatio;

		if (fixLength)
		{
			capsule.FixLength();
		}

		return true;
	}

	return false;
}

bool PushCapsuleOutOfPlane3D_Double(DoubleCapsule3& capsule, DoublePlane3 const& plane, bool fixLength)
{
	bool result = false;

	DoubleVec3 Q = plane.m_normal * plane.m_distanceFromOrigin;
	double dotEnd = DotProduct3D_Double(Q - capsule.m_end, plane.m_normal);
	double dotStart = DotProduct3D_Double(Q - capsule.m_start, plane.m_normal);

	if (dotEnd * dotStart <= 0.f)
	{
		result = true;
		double alt = 0;
		double alt1 = 0;
		double alt2 = 0;
		if (dotEnd >= 0)
		{
			alt1 = plane.GetAltitudeOfPoint(capsule.m_end);
			alt1 = fabs(alt1);
		}
		if (dotStart >= 0)
		{
			alt2 = plane.GetAltitudeOfPoint(capsule.m_start);
			alt2 = fabs(alt2);
		}
		alt = (alt1 > alt2) ? alt1 : alt2;
		alt += capsule.m_radius;

		capsule.m_end += plane.m_normal * alt;
		capsule.m_start += plane.m_normal * alt;
	}

	DoubleVec3 nearestStart = plane.GetNearestPoint(capsule.m_start);
	result |= PushCapsuleOutOfPoint3D_Double(capsule, nearestStart, fixLength);

	DoubleVec3 nearestEnd = plane.GetNearestPoint(capsule.m_end);
	result |= PushCapsuleOutOfPoint3D_Double(capsule, nearestEnd, fixLength);

	return result;
}

bool PushCapsuleOutOfAABB3D_Double(DoubleCapsule3& capsule, DoubleAABB3 const& aabb, bool fixLength)
{
	bool returnValue = false;

	if (DoSphereAndAABBOverlap3D_Double(capsule.m_start, capsule.m_radius, aabb))
	{
		DoubleVec3 toSphereCenter = (capsule.m_start - aabb.GetNearestPoint(capsule.m_start));
		double toSphereDist = toSphereCenter.GetLength();
		DoubleVec3 penDir = toSphereCenter.GetNormalized() * (capsule.m_radius - toSphereDist);
		double axisLength = capsule.GetAxisLength();
		double pushStartRatio = toSphereDist / axisLength;
		double pushEndRatio = (axisLength - toSphereDist) / axisLength;

		capsule.m_start += penDir * pushEndRatio;
		capsule.m_end += penDir * pushStartRatio;
		returnValue = true;
	}

	if (DoSphereAndAABBOverlap3D_Double(capsule.m_end, capsule.m_radius, aabb))
	{
		DoubleVec3 toSphereCenter = (capsule.m_end - aabb.GetNearestPoint(capsule.m_end));
		double toSphereDist = toSphereCenter.GetLength();
		DoubleVec3 penDir = toSphereCenter.GetNormalized() * (capsule.m_radius - toSphereDist);
		double axisLength = capsule.GetAxisLength();
		double pushStartRatio = toSphereDist / axisLength;
		double pushEndRatio = (axisLength - toSphereDist) / axisLength;

		capsule.m_start += penDir * pushStartRatio;
		capsule.m_end += penDir * pushEndRatio;
		returnValue = true;
	}

	if (returnValue && fixLength)
	{
		capsule.FixLength();
	}

	// Best Possible Intersects
	double tValues[6];
	double bestTValue = DBL_MAX;
	double smallestDistanceSquared = DBL_MAX;
	DoubleVec3 bestIntersectPoint = DoubleVec3(-DBL_MAX, -DBL_MAX, -DBL_MAX);
	DoubleVec3 bestNearestPointAABB = DoubleVec3(-DBL_MAX, -DBL_MAX, -DBL_MAX);
	DoubleVec3 axis = capsule.GetAxis();
	double rayZScale = 1.0 / axis.z;
	double rayYScale = 1.0 / axis.y;
	double rayXScale = 1.0 / axis.x;
	tValues[0] = ((aabb.m_maxs.z - capsule.m_start.z) * rayZScale);
	tValues[1] = ((aabb.m_mins.z - capsule.m_start.z) * rayZScale);
	tValues[2] = ((aabb.m_maxs.y - capsule.m_start.y) * rayYScale);
	tValues[3] = ((aabb.m_mins.y - capsule.m_start.y) * rayYScale);
	tValues[4] = ((aabb.m_maxs.x - capsule.m_start.x) * rayXScale);
	tValues[5] = ((aabb.m_mins.x - capsule.m_start.x) * rayXScale);

	for (double tValue : tValues)
	{
		if (tValue >= 0.0 && tValue <= 1.0)
		{
			DoubleVec3 intersectPoint = capsule.m_start + (tValue * axis);
			DoubleVec3 nearestPointAABB = aabb.GetNearestEdgePosition(intersectPoint);
			double distanceSquared = GetDistanceSquared3D_Double(intersectPoint, nearestPointAABB);
			if (distanceSquared < smallestDistanceSquared)
			{
				smallestDistanceSquared = distanceSquared;
				bestIntersectPoint = intersectPoint;
				bestNearestPointAABB = nearestPointAABB;
				bestTValue = tValue;
			}
		}
	}

	// Doesn't hit
	if (bestTValue < -1.0f || bestTValue > 2.0f)
	{
		return returnValue;
	}

	DoubleVec3 center = aabb.GetCenter();
	DoubleVec3 nearestPointBone = GetNearestPointOnLineSegment3D_Double(capsule.m_start, capsule.m_end, bestNearestPointAABB);
	bestNearestPointAABB = aabb.GetNearestEdgePosition(nearestPointBone);

	if (IsPointInsideAABB3D_Double(nearestPointBone, aabb))
	{
		DoubleVec3 displacementCenterToBone = nearestPointBone - center;
		DoubleVec3 displacementCenterToCorner = bestNearestPointAABB - center;

		if (displacementCenterToBone.GetLengthSquared() < displacementCenterToCorner.GetLengthSquared())
		{
			DoubleVec3 displacement = bestNearestPointAABB - nearestPointBone;
			displacement.SetLength(displacement.GetLength() + capsule.m_radius);
			capsule.m_start += displacement;
			capsule.m_end += displacement;
			if (fixLength)
			{
				capsule.FixLength();
			}
			return true;
		}
	}
	else
	{
		DoubleVec3 displacement = nearestPointBone - bestNearestPointAABB;
		double distanceSquared = displacement.GetLengthSquared();
		if (distanceSquared < capsule.m_radius * capsule.m_radius)
		{
			double offset = capsule.m_radius - sqrt(distanceSquared);
			displacement.SetLength(offset);
			capsule.m_start += displacement;
			capsule.m_end += displacement;
			if (fixLength)
			{
				capsule.FixLength();
			}
			return true;
		}
	}

	if (returnValue && fixLength)
	{
		//capsule.FixLength();
	}

	return returnValue;
}

bool PushCapsuleOutOfOBB3D_Double(DoubleCapsule3& capsule, DoubleOBB3 const& obb, bool fixLength)
{
	DoubleVec3 kBasisNormal = CrossProduct3D_Double(obb.m_iBasisNormal, obb.m_jBasisNormal);

	DoubleMat44 obbLtoW;
	obbLtoW.SetIJKT3D(obb.m_iBasisNormal, obb.m_jBasisNormal, kBasisNormal, obb.m_center);
	DoubleMat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	DoubleAABB3 aabb;
	aabb.m_mins = DoubleVec3(-obb.m_halfDimensions.x, -obb.m_halfDimensions.y, -obb.m_halfDimensions.z);
	aabb.m_maxs = DoubleVec3(obb.m_halfDimensions.x, obb.m_halfDimensions.y, obb.m_halfDimensions.z);

	DoubleCapsule3 localCapsule;
	localCapsule.m_radius = capsule.m_radius;
	localCapsule.m_start = obbWtoL.TransformPosition3D(capsule.m_start);
	localCapsule.m_end = obbWtoL.TransformPosition3D(capsule.m_end);

	if (PushCapsuleOutOfAABB3D_Double(localCapsule, aabb, false))
	{
		capsule.m_start = obbLtoW.TransformPosition3D(localCapsule.m_start);
		capsule.m_end = obbLtoW.TransformPosition3D(localCapsule.m_end);
		if (fixLength)
		{
			capsule.FixLength();
		}
		return true;
	}

	return false;
}

bool PushCapsuleOutOfCapsule3D_Double(DoubleCapsule3& capsuleA, DoubleCapsule3& capsuleB, bool isBStatic)
{
	//std::vector<DoubleVec3> nearestPointsOn2Bones = GetNearestPointsBetweenLines3D_Double(capsuleA.GetBone(), capsuleB.GetBone());
	//if (nearestPointsOn2Bones.empty())
	//{
	//	return false;
	//}

	DoubleVec3 nearestPointsOn2Bones[2];
	Optimized_GetNearestPointsBetweenLines3D_Double(nearestPointsOn2Bones, capsuleA.GetBone(), capsuleB.GetBone());

	DoubleVec3 dist2Points = nearestPointsOn2Bones[0] - nearestPointsOn2Bones[1];
	double dist2PointsLengthSquared = dist2Points.GetLengthSquared();

	if (dist2PointsLengthSquared < (capsuleA.m_radius + capsuleB.m_radius) * (capsuleA.m_radius + capsuleB.m_radius))
	{
		double penDepth = capsuleA.m_radius + capsuleB.m_radius - sqrt(dist2PointsLengthSquared);
		DoubleVec3 pushDir = dist2Points * penDepth;

		if (isBStatic)
		{
			double axisLength = capsuleA.GetAxisLength();
			DoubleVec3 startToNearestPoint = nearestPointsOn2Bones[0] - capsuleA.m_start;
			double startToNearestPointLength = startToNearestPoint.GetLength();

			double pushStartRatio = startToNearestPointLength / axisLength;
			double pushEndRatio = (axisLength - startToNearestPointLength) / axisLength;

			capsuleA.m_start += pushDir * pushEndRatio;
			capsuleA.m_end += pushDir * pushStartRatio;
		}
		else
		{
			double axisALength = capsuleA.GetAxisLength();
			DoubleVec3 startAToNearestPoint = nearestPointsOn2Bones[0] - capsuleA.m_start;
			double startAToNearestPointLength = startAToNearestPoint.GetLength();

			double pushAStartRatio = startAToNearestPointLength / axisALength;
			double pushAEndRatio = (axisALength - startAToNearestPointLength) / axisALength;

			capsuleA.m_start += pushDir * pushAEndRatio * 0.5;
			capsuleA.m_end += pushDir * pushAStartRatio * 0.5;


			double axisBLength = capsuleB.GetAxisLength();
			DoubleVec3 startBToNearestPoint = nearestPointsOn2Bones[1] - capsuleB.m_start;
			double startBToNearestPointLength = startBToNearestPoint.GetLength();

			double pushBStartRatio = startBToNearestPointLength / axisBLength;
			double pushBEndRatio = (axisBLength - startBToNearestPointLength) / axisBLength;

			capsuleB.m_start -= pushDir * pushBEndRatio * 0.5;
			capsuleB.m_end -= pushDir * pushBStartRatio * 0.5;
		}
		return true;
	}
	return false;
}

bool PushCapsuleOutOfSphere3D_Double(DoubleCapsule3& capsule, DoubleVec3& sCenter, double sRadius, bool isSphereStatic)
{
	DoubleVec3 nearestPointToBone = GetNearestPointOnLineSegment3D_Double(capsule.m_start, capsule.m_end, sCenter);
	DoubleVec3 fromSphereCenterToBone = nearestPointToBone - sCenter;
	double distanceFromSphereToBoneSquared = fromSphereCenterToBone.GetLengthSquared();
	if (distanceFromSphereToBoneSquared < (capsule.m_radius + sRadius) * (capsule.m_radius + sRadius))
	{
		double penetrationLength = (capsule.m_radius + sRadius) - sqrt(distanceFromSphereToBoneSquared);
		DoubleVec3 pushedNearestPointToBone = nearestPointToBone + fromSphereCenterToBone.GetNormalized() * penetrationLength;
		DoubleVec3 displacement = pushedNearestPointToBone - nearestPointToBone;

		double distanceStart = (nearestPointToBone - capsule.m_start).GetLength();
		double distanceEnd = (nearestPointToBone - capsule.m_end).GetLength();
		double totalDistance = capsule.GetAxisLength();

		double rateStart = distanceStart / totalDistance;
		double rateEnd = distanceEnd / totalDistance;

		if (isSphereStatic)
		{
			// flip end start ratio to make it look more real
			capsule.m_start += displacement * rateEnd;
			capsule.m_end += displacement * rateStart;
		}
		else
		{
			capsule.m_start += displacement * 0.5 * rateEnd;
			capsule.m_end += displacement * 0.5 * rateStart;
			sCenter -= displacement * 0.5;
		}

		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------------------------
//Transform Utilities

void TransformPosition2D(Vec2& pos, float uniformScale, float rotationDegrees, Vec2 const& translation)
{
	float rotationRadians = ConvertDegreesToRadians(rotationDegrees);
	float dist = pos.GetLength();
	float thetaRadians = atan2f(pos.y, pos.x);
	dist *= uniformScale;
	thetaRadians += rotationRadians;
	pos.x = translation.x + (dist * cosf(thetaRadians));
	pos.y = translation.y + (dist * sinf(thetaRadians));
}
//..............................
void TransformPosition2D(Vec2& posToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	posToTransform = translation + posToTransform.x * iBasis + posToTransform.y * jBasis;
}
//..............................
void TransformPositionXY3D(Vec3& pos, float scaleXY, float zRotationDegrees, Vec2 const& translation)
{
	float rotationRadians = ConvertDegreesToRadians(zRotationDegrees);
	float dist = pos.GetLengthXY();
	float thetaRadians = atan2f(pos.y, pos.x);
	dist *= scaleXY;
	thetaRadians += rotationRadians;
	pos.x = translation.x + (dist * cosf(thetaRadians));
	pos.y = translation.y + (dist * sinf(thetaRadians));
}
//..............................
void TransformPositionXY3D(Vec3& positionToTransform, Vec2 const& iBasis, Vec2 const& jBasis, Vec2 const& translation)
{
	Vec2 transitionX = positionToTransform.x * iBasis;
	Vec2 transitionY = positionToTransform.y * jBasis;
	positionToTransform = Vec3(translation.x + transitionX.x + transitionY.x, translation.y + transitionX.y + transitionY.y, positionToTransform.z);
}

//..............................
float Clamp(float value, float minValue, float maxValue)
{
	float result = value;
	if (value < minValue) result = minValue;
	if (value > maxValue) result = maxValue;
	return result;
}

EulerAngles Clamp(EulerAngles value, EulerAngles minValue, EulerAngles maxValue)
{
	EulerAngles result;
	result.m_yawDegrees = Clamp(value.m_yawDegrees, minValue.m_yawDegrees, maxValue.m_yawDegrees);
	result.m_pitchDegrees = Clamp(value.m_pitchDegrees, minValue.m_pitchDegrees, maxValue.m_pitchDegrees);
	result.m_rollDegrees = Clamp(value.m_rollDegrees, minValue.m_rollDegrees, maxValue.m_rollDegrees);

	return result;
}

Rgba8 Clamp(Rgba8 value, Rgba8 minValue, Rgba8 maxValue)
{
	Rgba8 result;

	result.r = (unsigned char)Clamp(value.r, minValue.r, maxValue.r);
	result.g = (unsigned char)Clamp(value.g, minValue.g, maxValue.g);
	result.b = (unsigned char)Clamp(value.b, minValue.b, maxValue.b);
	result.a = (unsigned char)Clamp(value.a, minValue.a, maxValue.a);

	return result;
}

Vec3 Clamp(Vec3 value, Vec3 minValue, Vec3 maxValue)
{
	Vec3 result;
	result.x = Clamp(value.x, minValue.x, maxValue.x);
	result.y = Clamp(value.y, minValue.y, maxValue.y);
	result.z = Clamp(value.z, minValue.z, maxValue.z);

	return result;
}

Vec2 Clamp(Vec2 value, Vec2 minValue, Vec2 maxValue)
{
	Vec2 result;
	result.x = Clamp(value.x, minValue.x, maxValue.x);
	result.y = Clamp(value.y, minValue.y, maxValue.y);

	return result;
}

double Clamp_Double(double value, double minValue, double maxValue)
{
	double result = value;
	if (value < minValue) result = minValue;
	if (value > maxValue) result = maxValue;
	return result;
}

//..............................
float ClampZeroToOne(float value)
{
	float result = value;
	if (value < 0) result = 0;
	if (value > 1) result = 1;
	return result;
}
//..............................
float Interpolate(float start, float end, float fraction)
{
	return start + fraction * (end - start);
}
Rgba8 Interpolate(Rgba8 start, Rgba8 end, float zeroToOne)
{
	Rgba8 result;
	result.r = FromFloatToUnsignedChar(Interpolate(FromUnsignedCharToFloat(start.r), FromUnsignedCharToFloat(end.r), zeroToOne));
	result.g = FromFloatToUnsignedChar(Interpolate(FromUnsignedCharToFloat(start.g), FromUnsignedCharToFloat(end.g), zeroToOne));
	result.b = FromFloatToUnsignedChar(Interpolate(FromUnsignedCharToFloat(start.b), FromUnsignedCharToFloat(end.b), zeroToOne));
	result.a = FromFloatToUnsignedChar(Interpolate(FromUnsignedCharToFloat(start.a), FromUnsignedCharToFloat(end.a), zeroToOne));
	return result;
}

Vec2 Interpolate(Vec2 start, Vec2 end, float zeroToOne)
{
	float x = Interpolate(start.x, end.x, zeroToOne);
	float y = Interpolate(start.y, end.y, zeroToOne);
	return Vec2(x, y);
}

Vec3 Interpolate(Vec3 start, Vec3 end, float zeroToOne)
{
	Vec3 result;
	result.x = Interpolate(start.x, end.x, zeroToOne);
	result.y = Interpolate(start.y, end.y, zeroToOne);
	result.z = Interpolate(start.z, end.z, zeroToOne);
	return result;
}

DoubleVec3 Interpolate(DoubleVec3 start, DoubleVec3 end, double zeroToOne)
{
	DoubleVec3 result;
	result.x = Interpolate(start.x, end.x, zeroToOne);
	result.y = Interpolate(start.y, end.y, zeroToOne);
	result.z = Interpolate(start.z, end.z, zeroToOne);
	return result;
}

double Interpolate(double start, double end, double fraction)
{
	return start + fraction * (end - start);
}

//..............................
float GetFractionWithinRange(float value, float rangeStart, float rangeEnd)
{
	return (value - rangeStart) / (rangeEnd - rangeStart);
}
//..............................
float RangeMap(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float t = GetFractionWithinRange(inValue, inStart, inEnd);
	return Interpolate(outStart, outEnd, t);
}
//..............................
float RangeMapClamped(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	float t = GetFractionWithinRange(inValue, inStart, inEnd);
	t = Clamp(t, 0.0f, 1.0f);
	return Interpolate(outStart, outEnd, t);
}

Vec2 RangeMapClamped(Vec2 inValue, Vec2 inStart, Vec2 inEnd, Vec2 outStart, Vec2 outEnd)
{
	Vec2 result;
	result.x = RangeMapClamped(inValue.x, inStart.x, inEnd.x, outStart.x, outEnd.x);
	result.y = RangeMapClamped(inValue.y, inStart.y, inEnd.y, outStart.y, outEnd.y);

	return result;
}

//..............................
int RoundDownToInt(float value)
{
	return static_cast<int>(floorf(value));
}
//..............................
float DotProduct2D(Vec2 const& a, Vec2 const& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

double DotProduct2D_Double(DoubleVec2 const& a, DoubleVec2 const& b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float DotProduct3D(Vec3 const& a, Vec3 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

double DotProduct3D_Double(DoubleVec3 const& a, DoubleVec3 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float DotProduct4D(Vec4 const& a, Vec4 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

double DotProduct4D_Double(DoubleVec4 const& a, DoubleVec4 const& b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

float CrossProduct2D(Vec2 A, Vec2 B)
{
	return A.x * B.y - A.y * B.x;
}

double CrossProduct2D_Double(DoubleVec2 A, DoubleVec2 B)
{
	return A.x * B.y - A.y * B.x;
}

Vec3 CrossProduct3D(Vec3 A, Vec3 B)
{
	Vec3 result;
	result.x = A.y * B.z - A.z * B.y;
	result.y = -(A.x * B.z - A.z * B.x);
	result.z = A.x * B.y - A.y * B.x;
	return result;
}

DoubleVec3 CrossProduct3D_Double(DoubleVec3 A, DoubleVec3 B)
{
	DoubleVec3 result;
	result.x = A.y * B.z - A.z * B.y;
	result.y = -(A.x * B.z - A.z * B.x);
	result.z = A.x * B.y - A.y * B.x;
	return result;
}

DoubleVec3 GetReflected3D_Double(DoubleVec3 orginal, DoubleVec3 normal)
{
	DoubleVec3 nNormal = normal.GetNormalized();
	return 2 * (nNormal.Dot(orginal)) * nNormal - orginal;
}

Quaternion FindLookAtRotationQuaternion(Vec3 Start, Vec3 Target)
{
	// Calculate forward direction
	Vec3 forward = (Target - Start).GetNormalized();

	// Define up vector (usually world up)
	Vec3 worldUp(0.0f, 1.0f, 0.0f);

	// Handle case where forward is parallel to world up
	if (fabsf(forward.Dot(worldUp)) > 0.9999f)
	{
		// Forward is nearly parallel to world up
		if (forward.y > 0)
		{
			// Looking straight up
			return Quaternion(0.0f, 0.0f, 0.0f, 1.0f); // Identity quaternion
		}
		else
		{
			// Looking straight down
			return Quaternion(1.0f, 0.0f, 0.0f, 0.0f); // 180 degree rotation around X axis
		}
	}

	// Calculate right vector
	Vec3 left = worldUp.Cross(forward).GetNormalized();

	// Recalculate up vector to ensure orthogonality
	Vec3 up = forward.Cross(left);

	// Construct rotation matrix
	Mat44 rotationMatrix;
	rotationMatrix.SetIJK3D(forward, left, up);

	// Convert rotation matrix to quaternion
	return rotationMatrix.GetQuaternion();
}

DoubleQuaternion FindLookAtRotationQuaternionDouble(DoubleVec3 Start, DoubleVec3 Target)
{
	// Calculate forward direction
	DoubleVec3 forward = (Target - Start).GetNormalized();

	// Define up vector (usually world up)
	DoubleVec3 worldUp(0.0, 1.0, 0.0);

	// Handle case where forward is parallel to world up
	if (fabs(forward.Dot(worldUp)) > 0.9999)
	{
		// Forward is nearly parallel to world up
		if (forward.y > 0)
		{
			// Looking straight up
			return DoubleQuaternion(0.0, 0.0, 0.0, 1.0); // Identity quaternion
		}
		else
		{
			// Looking straight down
			return DoubleQuaternion(1.0, 0.0, 0.0, 0.0); // 180 degree rotation around X axis
		}
	}

	// Calculate right vector
	DoubleVec3 left = worldUp.Cross(forward).GetNormalized();

	// Recalculate up vector to ensure orthogonality
	DoubleVec3 up = forward.Cross(left);

	// Construct rotation matrix
	DoubleMat44 rotationMatrix;
	rotationMatrix.SetIJK3D(forward, left, up);

	// Convert rotation matrix to quaternion
	return rotationMatrix.GetDoubleQuaternion();
}

EulerAngles FindLookAtRotationEulerAngle(Vec3 Start, Vec3 Target)
{
	// Calculate forward direction
	Vec3 forward = (Target - Start).GetNormalized();

	// Calculate pitch (rotation around X-axis)
	float pitch = -asinf(forward.y);

	// Calculate yaw (rotation around Y-axis)
	float yaw = atan2f(forward.x, forward.z);

	// Convert radians to degrees
	pitch = ConvertRadiansToDegrees(pitch);
	yaw = ConvertRadiansToDegrees(yaw);

	// Roll is typically 0 for a look-at rotation
	float roll = 0.0f;

	return EulerAngles(yaw, pitch, roll);
}

float NormalizeByte(unsigned char byte)
{
	if (byte <= 0)
	{
		return 0.f;
	}
	if (byte >= 255)
	{
		return 1.f;
	}

	return static_cast<float>(byte) / 255.f;
}

unsigned char DenormalizeByte(float f)
{
	if (f <= 0.f)
	{
		return 0;
	}
	if (f >= 1.f)
	{
		return 255;
	}

	return static_cast<unsigned char>(f * 256.f);
}

Mat44 GetBillboardMatrix(BilboardType type, Mat44 const& cameraMatrix, const Vec3& billboardPosition, const Vec2& billboardScale)
{
	Mat44 transform;

	transform.SetTranslation3D(billboardPosition);

	switch (type)
	{
	case BilboardType::NONE:
		break;
	case BilboardType::WORLD_UP_CAMERA_FACING:
		transform.LookAtTargetXY(cameraMatrix.GetTranslation3D());
		break;
	case BilboardType::WORLD_UP_CAMERA_OPPOSING:
		transform.SetIJK3D(cameraMatrix.GetIBasis3D() * -1.f, cameraMatrix.GetJBasis3D() * -1.f, Vec3(0, 0, 1));
		break;
	case BilboardType::FULL_CAMERA_FACING:
		transform.LookAtTarget(cameraMatrix.GetTranslation3D());
		break;
	case BilboardType::FULL_CAMERA_OPPOSING:
		transform.SetIJK3D(cameraMatrix.GetIBasis3D() * -1.f, cameraMatrix.GetJBasis3D() * -1.f, cameraMatrix.GetKBasis3D());
		break;
	default:
		break;
	}

	transform.AppendScaleNonUniform2D(billboardScale);
	return transform;
}

float SmoothStart2(float t)
{
	return t * t;
}

float SmoothStart3(float t)
{
	return t * t * t;
}

float SmoothStart4(float t)
{
	return  t * t * t * t;
}

float SmoothStart5(float t)
{
	return t * t * t * t * t;
}

float SmoothStart6(float t)
{
	return  t * t * t * t * t * t;
}

float SmoothStop2(float t)
{
	float s = 1.f - t;
	return 1 - s * s;
}

float SmoothStop3(float t)
{
	float s = 1.f - t;
	return 1 - s * s * s;
}

float SmoothStop4(float t)
{
	float s = 1.f - t;
	return 1 - s * s * s * s;
}

float SmoothStop5(float t)
{
	float s = 1.f - t;
	return 1 - s * s * s * s * s;
}

float SmoothStop6(float t)
{
	float s = 1.f - t;
	return 1 - s * s * s * s * s * s;
}

float SmoothStep3(float t)
{
	float a = SmoothStart2(t);
	float b = SmoothStop2(t);

	return (1 - t) * a + t * b;
}

float SmoothStep5(float t)
{
	float a = SmoothStart4(t);
	float b = SmoothStop4(t);

	return (1 - t) * a + t * b;
}

float Hesitate3(float t)
{
	return ComputeCubicBezier1D(t, 0, 1, 0, 1);
}

float Hesitate5(float t)
{
	return ComputeQuinticBezier1D(t, 0, 1, 0, 1, 0, 1);
}

bool FloatEqual(float a, float compareTo, float tolerance)
{
	return a < compareTo + tolerance && a > compareTo - tolerance;
}

bool DoubleEqual(double a, double compareTo, double tolerance)
{
	return a < compareTo + tolerance && a > compareTo - tolerance;
}

float FloatMin(float a, float b)
{
	return (a < b) ? a : b;
}

float FloatMax(float a, float b)
{
	return (a > b) ? a : b;
}

int IntMin(int a, int b)
{
	return (a < b) ? a : b;
}

int IntMax(int a, int b)
{
	return (a > b) ? a : b;
}

double DoubleMin(double a, double b)
{
	return (a < b) ? a : b;
}

double DoubleMax(double a, double b)
{
	return (a > b) ? a : b;
}

float ComputeCubicBezier1D(float t, float p0, float p1, float p2, float p3)
{
	float s = 1.f - t;

	float A1 = s * p0 + t * p1;
	float A2 = s * p1 + t * p2;
	float A3 = s * p2 + t * p3;

	float B1 = s * A1 + t * A2;
	float B2 = s * A2 + t * A3;

	float C1 = s * B1 + t * B2;

	return C1;
}

float ComputeQuinticBezier1D(float t, float p0, float p1, float p2, float p3, float p4, float p5)
{
	float s = 1.f - t;

	float A1 = s * p0 + t * p1;
	float A2 = s * p1 + t * p2;
	float A3 = s * p2 + t * p3;
	float A4 = s * p3 + t * p4;
	float A5 = s * p4 + t * p5;

	float B1 = s * A1 + t * A2;
	float B2 = s * A2 + t * A3;
	float B3 = s * A3 + t * A4;
	float B4 = s * A4 + t * A5;

	float C1 = s * B1 + t * B2;
	float C2 = s * B2 + t * B3;
	float C3 = s * B3 + t * B4;

	float D1 = s * C1 + t * C2;
	float D2 = s * C2 + t * C3;

	float E1 = s * D1 + t * D2;

	return E1;
}
bool BounceDiscOffPoint(Vec2& discPosition, float discRadius, Vec2& discVelocity, float discElasticity, Vec2& pointPosition, float pointElasticity)
{
	if (!PushDiscOutOfPoint2D(discPosition, discRadius, pointPosition))
	{
		return false;
	}

	float eTotal = discElasticity * pointElasticity;
	Vec2 n = (discPosition - pointPosition).GetNormalized();
	float nLength = DotProduct2D(discVelocity, n);
	Vec2 nDisc = n * nLength;
	Vec2 t = discVelocity - nDisc;
	discVelocity = t + nDisc * eTotal;

	return true;
}

bool BounceDiscOffStaticDisc2D(Vec2& mobileDiscPosition, float mobileDiscRadius, Vec2& mobileDiscVelocity, float mobileDiscElasticity, const Vec2& staticDiscPosition, float staticDiscRadius, float staticDiscElasticity)
{
	if (!PushDiscOutOfDisc2D(mobileDiscPosition, mobileDiscRadius, staticDiscPosition, staticDiscRadius))
	{
		return false;
	}

	float eTotal = mobileDiscElasticity * staticDiscElasticity;

	Vec2 n = (mobileDiscPosition - staticDiscPosition).GetNormalized();
	float nLength = DotProduct2D(mobileDiscVelocity, n);
	Vec2 nMobile = n * nLength;
	Vec2 t = mobileDiscVelocity - nMobile;
	mobileDiscVelocity = t + nMobile * eTotal;

	return true;
}

bool BounceDiscOffEachOther2D(Vec2& discAPosition, float discARadius, Vec2& discAVelocity, float discAElasticity, Vec2& discBPosition, float discBRadius, Vec2& discBVelocity, float discBElasticity)
{
	if (!PushDiscsOutOfEachOther2D(discAPosition, discARadius, discBPosition, discBRadius))
	{
		return false;
	}

	Vec2 n = (discBPosition - discAPosition).GetNormalized();

	float NALength = DotProduct2D(discAVelocity, n);
	float NBLength = DotProduct2D(discBVelocity, n);

	if (NBLength - NALength > 0)
	{
		return false;
	}

	float eTotal = discAElasticity * discBElasticity;

	Vec2 NA = n * NALength;
	Vec2 NB = n * NBLength;

	Vec2 tA = discAVelocity - NA;
	Vec2 tB = discBVelocity - NB;

	discAVelocity = tA + NB * eTotal;
	discBVelocity = tB + NA * eTotal;

	return true;
}

bool BounceSphereOffPoint(Vec3& spherePosition, float sphereRadius, Vec3& sphereVelocity, float sphereElasticity, Vec3 pointPosition, float pointElasticity, float pointFriction)
{
	if (!PushSphereOutOfPoint3D(spherePosition, sphereRadius, pointPosition))
	{
		return false;
	}

	float eTotal = sphereElasticity * pointElasticity;
	Vec3 n = (spherePosition - pointPosition).GetNormalized();
	float nLength = DotProduct3D(sphereVelocity, n);
	Vec3 nDisc = n * nLength;
	Vec3 t = (sphereVelocity - nDisc) * (1.f - pointFriction);
	sphereVelocity = t - nDisc * eTotal;

	return true;
}

bool BounceSphereOffStaticSphere3D(Vec3& mobileSpherePosition, float mobileSphereRadius, Vec3& mobileSphereVelocity, float mobileSphereElasticity, const Vec3& staticSpherePosition, float staticSphereRadius, float staticSphereElasticity)
{
	Vec3 staticSpherePos = staticSpherePosition;
	if (!PushSphereOutOfSphere3D(mobileSpherePosition, mobileSphereRadius, staticSpherePos, staticSphereRadius, true))
	{
		return false;
	}

	float eTotal = mobileSphereElasticity * staticSphereElasticity;

	Vec3 n = (mobileSpherePosition - staticSpherePos).GetNormalized();
	float nLength = DotProduct3D(mobileSphereVelocity, n);
	Vec3 nMobile = n * nLength;
	Vec3 t = mobileSphereVelocity - nMobile;
	mobileSphereVelocity = t + nMobile * eTotal;

	return true;
}

bool BounceSphereOffEachOther3D(Vec3& posA, Vec3& posB, float radiusA, float radiusB, Vec3& velA, Vec3& velB, float elasticiyA, float elasticiyB, float massA, float massB)
{
	if (!PushSphereOutOfSphere3D(posA, radiusA, posB, radiusB, false))
	{
		return false;
	}

	Vec3 dist = posA - posB;
	float totalE = elasticiyA * elasticiyB;

	dist.Normalize();
	float velALengthOnDis = DotProduct3D(dist, velA);
	Vec3 velAX = dist * velALengthOnDis;
	Vec3 velAY = velA - velAX;

	dist = dist * -1;
	float velBLengthOnDis = DotProduct3D(dist, velB);
	Vec3 velBX = dist * velBLengthOnDis;
	Vec3 velBY = velB - velBX;

	float inverseTotalMass = 1.f / (massA + massB);

	velA = Vec3(velAX * (massA - massB) * inverseTotalMass + velBX * (2 * massB) * inverseTotalMass + velAY) * totalE;
	velB = Vec3(velAX * (2 * massA) * inverseTotalMass + velBX * (massB - massA) * inverseTotalMass + velBY) * totalE;

	return true;
}

bool BounceSphereOffPoint3D_Double(DoubleVec3& spherePosition, double sphereRadius, DoubleVec3& sphereVelocity, double sphereElasticity, DoubleVec3 pointPosition, double pointElasticity, double pointFriction /*= 0.f*/)
{
	if (!PushSphereOutOfPoint3D_Double(spherePosition, sphereRadius, pointPosition))
	{
		return false;
	}

	double eTotal = sphereElasticity * pointElasticity;
	DoubleVec3 n = (spherePosition - pointPosition).GetNormalized();
	double nLength = DotProduct3D(sphereVelocity, n);
	DoubleVec3 nDisc = n * nLength;
	DoubleVec3 t = (sphereVelocity - nDisc) * (1.f - pointFriction);
	sphereVelocity = t - nDisc * eTotal;

	return true;
}

bool BounceSphereOffStaticSphere3D_Double(DoubleVec3& mobileSpherePosition, double mobileSphereRadius, DoubleVec3& mobileSphereVelocity, double mobileSphereElasticity, const DoubleVec3& staticSpherePosition, double staticSphereRadius, double staticSphereElasticity)
{
	DoubleVec3 staticSpherePos = staticSpherePosition;
	if (!PushSphereOutOfSphere3D_Double(mobileSpherePosition, mobileSphereRadius, staticSpherePos, staticSphereRadius, true))
	{
		return false;
	}

	double eTotal = mobileSphereElasticity * staticSphereElasticity;

	DoubleVec3 n = (mobileSpherePosition - staticSpherePos).GetNormalized();
	double nLength = DotProduct3D(mobileSphereVelocity, n);
	DoubleVec3 nMobile = n * nLength;
	DoubleVec3 t = mobileSphereVelocity - nMobile;
	mobileSphereVelocity = t + nMobile * eTotal;

	return true;
}

bool BounceCapsuleOffPoint3D_Double(DoubleCapsule3& capsule, DoubleVec3& startPointVel, DoubleVec3& endPointVel, double elasticity, DoubleVec3 point, double pointElasticity, double pointFriction, bool fixLength)
{
	UNUSED(startPointVel);
	UNUSED(endPointVel);
	UNUSED(elasticity);
	UNUSED(pointElasticity);
	UNUSED(pointFriction);
	UNUSED(fixLength);
	if (!PushCapsuleOutOfPoint3D_Double(capsule, point, fixLength))
	{
		return false;
	}


	return true;
}

bool BounceCapsuleOffPlane3D_Double(DoubleCapsule3& capsule, DoubleVec3& startPointVel, DoubleVec3& endPointVel, double elasticity, DoublePlane3 plane, double planeElasticity, double planeFriction /*= 0.f*/, bool fixLength /*= true*/)
{
	UNUSED(elasticity);
	if (!PushCapsuleOutOfPlane3D_Double(capsule, plane, fixLength))
	{
		return false;
	}
	DoubleVec3 nearestStart = plane.GetNearestPoint(capsule.m_start);
	BounceCapsuleOffPoint3D_Double(capsule, startPointVel, endPointVel, planeElasticity, nearestStart, planeFriction, fixLength);

	DoubleVec3 nearestEnd = plane.GetNearestPoint(capsule.m_end);
	BounceCapsuleOffPoint3D_Double(capsule, startPointVel, endPointVel, planeElasticity, nearestEnd, planeFriction, fixLength);

	return true;
}

FloatRange ProjectVertices(std::vector<Vec2>& vertices, Vec2 axis)
{
	float min = axis.Dot(vertices[0]);
	float max = min;

	for (size_t i = 1; i < vertices.size(); i++)
	{
		float projection = axis.Dot(vertices[i]);
		min = FloatMin(min, projection);
		max = FloatMax(max, projection);
	}

	return FloatRange(min, max);
}

std::vector<Vec2> GetAxes(const std::vector<Vec2>& vertices)
{
	std::vector<Vec2> axes;
	axes.reserve(vertices.size());

	for (size_t i = 0; i < vertices.size(); i++) 
	{
		size_t j = (i + 1) % vertices.size();
		Vec2 edge(vertices[j].x - vertices[i].x, vertices[j].y - vertices[i].y);

		// Get perpendicular vector as normal
		Vec2 normal(-edge.y, edge.x);
		axes.push_back(normal.GetNormalized());
	}

	return axes;
}

bool DoAABBOverlapConvexPoly2D(AABB2 box, ConvexPoly2 convex)
{
	std::vector<Vec2> aabbVertices = box.GetVertices();
	std::vector<Vec2> convexVertices = convex.GetPositions();

	std::vector<Vec2> aabbAxes = GetAxes(aabbVertices);
	std::vector<Vec2> convexAxes = GetAxes(convexVertices);

	// Combine all axes
	std::vector<Vec2> axes;
	axes.reserve(aabbAxes.size() + convexAxes.size());
	axes.insert(axes.end(), aabbAxes.begin(), aabbAxes.end());
	axes.insert(axes.end(), convexAxes.begin(), convexAxes.end());

	// Test projection onto each axis
	for (const auto& axis : axes) 
	{
		auto aabbProj = ProjectVertices(aabbVertices, axis);
		auto convexProj = ProjectVertices(convexVertices, axis);

		if (aabbProj.m_max < convexProj.m_min || convexProj.m_max < aabbProj.m_min) 
		{
			// Separating axis found
			return false; 
		}
	}

	// No separating axis found = collision
	return true; 
}
