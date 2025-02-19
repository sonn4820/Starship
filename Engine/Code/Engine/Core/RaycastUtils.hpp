#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Plane3.hpp"
#include "Engine/Math/Capsule3.hpp"
#include "Engine/Math/ConvexShape.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Renderer/Camera.hpp"
#include <vector>

struct RaycastResult2D
{
	// Basic raycast result information (required)
	bool	m_didImpact = false;
	float	m_impactDist = 0.f;
	Vec2	m_impactPos;
	Vec2	m_impactNormal;

	// Original raycast information (optional)
	Vec2	m_rayFwdNormal;
	Vec2	m_rayStartPos;
	float	m_rayMaxLength = 1.f;
};

struct RaycastResult3D
{
	// Basic raycast result information (required)
	bool	m_didImpact = false;
	float	m_impactDist = 0.f;
	Vec3	m_impactPos;
	Vec3	m_impactNormal;

	// Original raycast information (optional)
	Vec3	m_rayFwdNormal;
	Vec3	m_rayStartPos;
	float	m_rayMaxLength = 1.f;
};

RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius);
RaycastResult2D RaycastVsLineSegments2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 linePointA, Vec2 linePointB);
RaycastResult2D RaycastVsAABB2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, AABB2 box);
RaycastResult2D RaycastVsPlane2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Plane2 plane);
RaycastResult2D RaycastVsConvex2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, ConvexHull2 convex);

RaycastResult3D RaycastVsAABB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, AABB3 box);
RaycastResult3D RaycastVsOBB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, OBB3 box);
RaycastResult3D RaycastVsPlane3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Plane3 plane);
RaycastResult3D RaycastVsSphere3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 sphereCenter, float sphereRadius);
RaycastResult3D RaycastVsCapsule3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Capsule3 capsule);
RaycastResult3D RaycastVsZCynlinder3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec2 cylCenterXY, FloatRange cylMinMaxZ, float cylRadius);

void MouseToRaycast(Camera* camera, Vec2 cursorPosition, Vec3& out_startPos, Vec3& out_forwardDir);
RaycastResult3D MouseRaycastVsPlane3D(Camera* camera, Vec2 cursorPosition, Plane3 plane);
RaycastResult3D MouseRaycastVsSphere3D(Camera* camera, Vec2 cursorPosition, Vec3 center, float radius);
RaycastResult3D MouseRaycastVsCapsule3D(Camera* camera, Vec2 cursorPosition, Capsule3 capsule);
