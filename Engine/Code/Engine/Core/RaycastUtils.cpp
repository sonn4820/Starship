#include "Engine/Core/RaycastUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/FloatRange.hpp"
#include "Engine/Math/Mat44.hpp"
#include "Engine/Renderer/DebugRender.hpp"

RaycastResult2D RaycastVsDisc2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 discCenter, float discRadius)
{
	RaycastResult2D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	if (IsPointInsideDisc2D(startPos, discCenter, discRadius))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	Vec2 toDiscCenter = discCenter - startPos;
	Vec2& iBasis = fwdNormal;
	Vec2 jBasis = iBasis.GetRotated90Degrees();
	float altitude = DotProduct2D(toDiscCenter, jBasis);

	if (altitude >= discRadius)
	{
		result.m_didImpact = false;
		return result;
	}
	if (altitude <= -discRadius)
	{
		result.m_didImpact = false;
		return result;
	}

	float distanceImpactPointOnRaycastLineToCenter = sqrtf((discRadius * discRadius) - (altitude * altitude));
	float distanceStartPointOnRaycastLineToCenter = DotProduct2D(toDiscCenter, iBasis);

	result.m_didImpact = true;
	result.m_impactDist = distanceStartPointOnRaycastLineToCenter - distanceImpactPointOnRaycastLineToCenter;
	result.m_impactPos = startPos + (iBasis * result.m_impactDist);
	result.m_impactNormal = (result.m_impactPos - discCenter).GetNormalized();

	if (result.m_impactDist > maxDist || result.m_impactDist < 0.0f)
	{
		result.m_didImpact = false;
		return result;
	}

	return result;
}

RaycastResult2D RaycastVsLineSegments2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Vec2 linePointA, Vec2 linePointB)
{
	RaycastResult2D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	Vec2 leftNormal = fwdNormal.GetRotatedMinus90Degrees();

	Vec2 toPointA = linePointA - startPos;
	Vec2 toPointB = linePointB - startPos;

	float dotPointAToRay = DotProduct2D(toPointA, leftNormal);
	float dotPointBToRay = DotProduct2D(toPointB, leftNormal);

	if (dotPointAToRay * dotPointBToRay >= 0.f)
	{
		result.m_didImpact = false;
		return result;
	}

	float fraction = dotPointBToRay / (dotPointBToRay - dotPointAToRay);
	Vec2 impactPoint = linePointB + fraction * (linePointA - linePointB);

	Vec2 toImpactPoint = impactPoint - startPos;
	float impactDistance = DotProduct2D(toImpactPoint, fwdNormal);

	if (impactDistance >= maxDist || impactDistance <= 0.f)
	{
		result.m_didImpact = false;
		return result;
	}

	Vec2 AtoB = linePointA - linePointB;
	Vec2 impactNormal = AtoB.GetRotated90Degrees();
	if (dotPointBToRay < 0.f)
	{
		impactNormal *= -1.f;
	}
	;

	result.m_didImpact = true;
	result.m_impactDist = impactDistance;
	result.m_impactPos = impactPoint;
	result.m_impactNormal = impactNormal.GetNormalized();

	return result;
}

RaycastResult2D RaycastVsAABB2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, AABB2 box)
{
	RaycastResult2D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	Vec2 endPos = startPos + maxDist * fwdNormal;
	AABB2 rayBox(startPos, startPos);
	rayBox.StretchToIncludePoint(endPos);

	if (!DoAABBsOverlap2D(rayBox, box))
	{
		result.m_didImpact = false;
		return result;
	}
	if (IsPointInsideAABB2D(startPos, box))
	{
		result.m_didImpact = true;
		result.m_impactDist = 0;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		return result;
	}

	FloatRange TxRange;
	FloatRange TyRange;
	bool xFlip = false;
	bool yFlip = false;

	if ((endPos.x - startPos.x) == 0)
	{
		TxRange.m_min = INFINITY * -1;
		TxRange.m_max = INFINITY;
	}
	else
	{
		float oneOverRangeX = 1.f / (endPos.x - startPos.x);
		TxRange.m_min = (box.m_mins.x - startPos.x) * oneOverRangeX;
		TxRange.m_max = (box.m_maxs.x - startPos.x) * oneOverRangeX;
	}

	if (TxRange.m_min > TxRange.m_max)
	{
		float temp = TxRange.m_min;
		TxRange.m_min = TxRange.m_max;
		TxRange.m_max = temp;
		xFlip = true;
	}

	if ((endPos.y - startPos.y) == 0)
	{
		TyRange.m_min = INFINITY * -1;
		TyRange.m_max = INFINITY;
	}
	else
	{
		float oneOverRangeY = 1.f / (endPos.y - startPos.y);
		TyRange.m_min = (box.m_mins.y - startPos.y) * oneOverRangeY;
		TyRange.m_max = (box.m_maxs.y - startPos.y) * oneOverRangeY;
	}


	if (TyRange.m_min > TyRange.m_max)
	{
		float temp = TyRange.m_min;
		TyRange.m_min = TyRange.m_max;
		TyRange.m_max = temp;
		yFlip = true;
	}

	if (TxRange.IsOverlappingWith(TyRange))
	{
		FloatRange tOverlapXandY;
		bool isRayHitOnSide;

		if (TxRange.m_min > TyRange.m_min)
		{
			tOverlapXandY.m_min = TxRange.m_min;
			isRayHitOnSide = true;
		}
		else
		{
			tOverlapXandY.m_min = TyRange.m_min;
			isRayHitOnSide = false;
		}

		if (TxRange.m_max > TyRange.m_max)
		{
			tOverlapXandY.m_max = TyRange.m_max;
		}
		else
		{
			tOverlapXandY.m_max = TxRange.m_max;
		}

		result.m_didImpact = true;
		result.m_impactDist = maxDist * tOverlapXandY.m_min;
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;

		Vec2 top = Vec2(box.m_maxs - result.m_impactPos).GetNormalized().GetRotated90Degrees();
		Vec2 bottom = Vec2(box.m_mins - result.m_impactPos).GetNormalized().GetRotated90Degrees();
		Vec2 right = Vec2(box.m_maxs - result.m_impactPos).GetNormalized().GetRotatedMinus90Degrees();
		Vec2 left = Vec2(box.m_mins - result.m_impactPos).GetNormalized().GetRotatedMinus90Degrees();

		if (isRayHitOnSide)
		{
			if (xFlip)
			{
				result.m_impactNormal = right;
			}
			else
			{
				result.m_impactNormal = left;
			}
		}
		else
		{
			if (yFlip)
			{
				result.m_impactNormal = top;
			}
			else
			{
				result.m_impactNormal = bottom;
			}
		}

	}
	else
	{
		result.m_didImpact = false;
		return result;
	}
	return result;
}

RaycastResult2D RaycastVsPlane2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, Plane2 plane)
{
	RaycastResult2D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	Vec2 endPoint = startPos + fwdNormal * maxDist;
	Vec2 Q = plane.m_normal * plane.m_distanceFromOrigin;

	float dotEnd = DotProduct2D(Q - endPoint, plane.m_normal);
	float dotStart = DotProduct2D(Q - startPos, plane.m_normal);

	if (dotEnd * dotStart < 0.f)
	{
		result.m_didImpact = true;
		result.m_impactDist = dotStart / DotProduct2D(plane.m_normal, fwdNormal);
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
		if (dotStart < 0)
		{
			result.m_impactNormal = plane.m_normal;
		}
		else
		{
			result.m_impactNormal = plane.m_normal * -1.f;
		}
	}

	return result;
}

RaycastResult2D RaycastVsConvex2D(Vec2 startPos, Vec2 fwdNormal, float maxDist, ConvexHull2 convex)
{
	RaycastResult2D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	Vec2 farthestEntry = startPos;
	Vec2 nearestExit = startPos + fwdNormal * maxDist;
	Plane2 hitPlane;

	for (size_t i = 0; i < convex.GetSize(); i++)
	{
		Plane2 plane = convex.GetPlanes()[i];

		RaycastResult2D localResult = RaycastVsPlane2D(startPos, fwdNormal, maxDist, plane);

		if (localResult.m_didImpact)
		{
			if (DotProduct2D(fwdNormal, plane.m_normal) < 0)
			{
				Vec2 entry = localResult.m_impactPos;

				if ((entry - startPos).GetLengthSquared() > (farthestEntry - startPos).GetLengthSquared())
				{
					farthestEntry = entry;
					hitPlane = plane;
				}
			}

			if (DotProduct2D(fwdNormal, plane.m_normal) > 0)
			{
				Vec2 exit = localResult.m_impactPos;

				if ((exit - startPos).GetLengthSquared() < (nearestExit - startPos).GetLengthSquared())
				{
					nearestExit = exit;
				}
			}
		}
	}

	if ((nearestExit - startPos).GetLengthSquared() < (farthestEntry - startPos).GetLengthSquared())
	{
		result.m_didImpact = false;
		return result;
	}

	Vec2 midpoint = farthestEntry + (nearestExit - farthestEntry) * 0.5f;

	if (convex.IsPointInside(midpoint))
	{
		result.m_didImpact = true;
		result.m_impactPos = farthestEntry;
		result.m_impactNormal = hitPlane.m_normal;
		result.m_impactDist = (farthestEntry - startPos).GetLength();

	}

	if (convex.IsPointInside(startPos))
	{
		result.m_didImpact = true;
		result.m_impactPos = startPos;
		result.m_impactNormal = -fwdNormal;
		result.m_impactDist = 0.f;
	}


	return result;
}

RaycastResult3D RaycastVsAABB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, AABB3 box)
{
	RaycastResult3D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	Vec3 endPos = startPos + fwdNormal * maxDist;
	AABB3 rayBox(startPos, startPos);
	rayBox.StretchToIncludePoint(endPos);
	if (!DoAABBsOverlap3D(rayBox, box))
	{
		result.m_didImpact = false;
		return result;
	}

	if (IsPointInsideAABB3D(startPos, box))
	{
		result.m_didImpact = true;
		result.m_impactPos = startPos;
		result.m_impactDist = 0;
		result.m_impactNormal = fwdNormal * -1.f;
		return result;
	}

	FloatRange TxRange;
	FloatRange TyRange;
	FloatRange TzRange;
	bool xFlip = false;
	bool yFlip = false;
	bool zFlip = false;

	if ((endPos.x - startPos.x) == 0)
	{
		TxRange.m_min = INFINITY * -1;
		TxRange.m_max = INFINITY;
	}
	else
	{
		float oneOverRangeX = 1.f / (endPos.x - startPos.x);
		TxRange.m_min = (box.m_mins.x - startPos.x) * oneOverRangeX;
		TxRange.m_max = (box.m_maxs.x - startPos.x) * oneOverRangeX;
	}

	if (TxRange.m_min > TxRange.m_max)
	{
		float temp = TxRange.m_min;
		TxRange.m_min = TxRange.m_max;
		TxRange.m_max = temp;
		xFlip = true;
	}

	if ((endPos.y - startPos.y) == 0)
	{
		TyRange.m_min = INFINITY * -1;
		TyRange.m_max = INFINITY;
	}
	else
	{
		float oneOverRangeY = 1.f / (endPos.y - startPos.y);
		TyRange.m_min = (box.m_mins.y - startPos.y) * oneOverRangeY;
		TyRange.m_max = (box.m_maxs.y - startPos.y) * oneOverRangeY;
	}


	if (TyRange.m_min > TyRange.m_max)
	{
		float temp = TyRange.m_min;
		TyRange.m_min = TyRange.m_max;
		TyRange.m_max = temp;
		yFlip = true;
	}

	if ((endPos.z - startPos.z) == 0)
	{
		TzRange.m_min = INFINITY * -1;
		TzRange.m_max = INFINITY;
	}
	else
	{
		float oneOverRangeZ = 1.f / (endPos.z - startPos.z);
		TzRange.m_min = (box.m_mins.z - startPos.z) * oneOverRangeZ;
		TzRange.m_max = (box.m_maxs.z - startPos.z) * oneOverRangeZ;
	}


	if (TzRange.m_min > TzRange.m_max)
	{
		float temp = TzRange.m_min;
		TzRange.m_min = TzRange.m_max;
		TzRange.m_max = temp;
		zFlip = true;
	}

	FloatRange OverlapRange;
	bool XOverlapY = TxRange.IsOverlappingWith(TyRange);
	bool XOverlapZ = TxRange.IsOverlappingWith(TzRange);
	bool YOverlapZ = TyRange.IsOverlappingWith(TzRange);
	bool isRayHitHorizontally = false;
	bool isRaySideFace = false;

	if (XOverlapY && XOverlapZ && YOverlapZ)
	{
		if (TxRange.m_min > TyRange.m_min)
		{
			OverlapRange.m_min = TxRange.m_min;
			isRayHitHorizontally = true;
			isRaySideFace = false;
		}
		else
		{
			OverlapRange.m_min = TyRange.m_min;
			isRayHitHorizontally = true;
			isRaySideFace = true;
		}
		if (OverlapRange.m_min < TzRange.m_min)
		{
			OverlapRange.m_min = TzRange.m_min;
			isRayHitHorizontally = false;
		}

		if (TxRange.m_max > TyRange.m_max)
		{
			OverlapRange.m_max = TyRange.m_max;
		}
		else
		{
			OverlapRange.m_max = TxRange.m_max;
		}
		if (OverlapRange.m_max > TzRange.m_max)
		{
			OverlapRange.m_max = TzRange.m_max;
		}

		result.m_didImpact = true;
		result.m_impactDist = maxDist * OverlapRange.m_min;
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;

		if (isRayHitHorizontally)
		{
			if (isRaySideFace)
			{
				Vec3 impactToMaxZ = Vec3(result.m_impactPos.x, result.m_impactPos.y, box.m_maxs.z) - result.m_impactPos;
				Vec3 impactToMaxX = Vec3(box.m_maxs.x, result.m_impactPos.y, result.m_impactPos.z) - result.m_impactPos;
				Vec3 impactToMinX = Vec3(box.m_mins.x, result.m_impactPos.y, result.m_impactPos.z) - result.m_impactPos;
				if (yFlip)
				{
					result.m_impactNormal = CrossProduct3D(impactToMaxZ, impactToMaxX);
				}
				else
				{
					result.m_impactNormal = CrossProduct3D(impactToMaxZ, impactToMinX);
				}
			}
			else
			{
				Vec3 impactToMinY = Vec3(result.m_impactPos.x, box.m_mins.y, result.m_impactPos.z) - result.m_impactPos;
				Vec3 impactToMaxY = Vec3(result.m_impactPos.x, box.m_maxs.y, result.m_impactPos.z) - result.m_impactPos;
				Vec3 impactToMaxZ = Vec3(result.m_impactPos.x, result.m_impactPos.y, box.m_maxs.z) - result.m_impactPos;
				if (xFlip)
				{
					result.m_impactNormal = CrossProduct3D(impactToMaxZ, impactToMinY);
				}
				else
				{
					result.m_impactNormal = CrossProduct3D(impactToMaxZ, impactToMaxY);
				}
			}

		}
		else
		{
			Vec3 impactToMaxY = Vec3(result.m_impactPos.x, box.m_maxs.y, result.m_impactPos.z) - result.m_impactPos;
			Vec3 impactToMinX = Vec3(box.m_mins.x, result.m_impactPos.y, result.m_impactPos.z) - result.m_impactPos;
			Vec3 impactToMaxX = Vec3(box.m_maxs.x, result.m_impactPos.y, result.m_impactPos.z) - result.m_impactPos;
			if (zFlip)
			{
				result.m_impactNormal = CrossProduct3D(impactToMaxY, impactToMinX);
			}
			else
			{
				result.m_impactNormal = CrossProduct3D(impactToMaxY, impactToMaxX);
			}
		}
		result.m_impactNormal.Normalize();
	}
	else
	{
		result.m_didImpact = false;
		return result;
	}

	return result;
}

RaycastResult3D RaycastVsOBB3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, OBB3 box)
{
	RaycastResult3D result;

	Vec3 kBasisNormal = CrossProduct3D(box.m_iBasisNormal, box.m_jBasisNormal);

	Mat44 obbLtoW;
	obbLtoW.SetIJKT3D(box.m_iBasisNormal, box.m_jBasisNormal, kBasisNormal, box.m_center);
	Mat44 obbWtoL = obbLtoW.GetOrthonormalInverse();

	Vec3 startPosInOBBSpace = obbWtoL.TransformPosition3D(startPos);
	Vec3 fwdNormalInOBBSpace = obbWtoL.TransformVectorQuantity3D(fwdNormal);

	Vec3 obbMin = Vec3(-box.m_halfDimensions.x, -box.m_halfDimensions.y, -box.m_halfDimensions.z);
	Vec3 obbMax = Vec3(box.m_halfDimensions.x, box.m_halfDimensions.y, box.m_halfDimensions.z);

	result = RaycastVsAABB3D(startPosInOBBSpace, fwdNormalInOBBSpace, maxDist, AABB3(obbMin, obbMax));

	if (result.m_didImpact)
	{
		result.m_impactPos = obbLtoW.TransformPosition3D(result.m_impactPos);
		result.m_impactNormal = obbLtoW.TransformVectorQuantity3D(result.m_impactNormal);
		result.m_impactDist = (result.m_impactPos - startPos).GetLength();
	}

	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	return result;
}

RaycastResult3D RaycastVsPlane3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Plane3 plane)
{
	RaycastResult3D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	Vec3 endPoint = startPos + fwdNormal * maxDist;
	Vec3 Q = plane.m_normal * plane.m_distanceFromOrigin;

	float dotEnd = DotProduct3D(Q - endPoint, plane.m_normal);
	float dotStart = DotProduct3D(Q - startPos, plane.m_normal);

	if (dotEnd * dotStart < 0.f)
	{
		result.m_didImpact = true;
		result.m_impactDist = dotStart / DotProduct3D(plane.m_normal, fwdNormal);
		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
		if (dotStart < 0)
		{
			result.m_impactNormal = plane.m_normal;
		}
		else
		{
			result.m_impactNormal = plane.m_normal * -1.f;
		}
	}

	return result;
}

RaycastResult3D RaycastVsSphere3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec3 sphereCenter, float sphereRadius)
{
	RaycastResult3D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	if (IsPointInsideSphere3D(startPos, sphereCenter, sphereRadius))
	{
		result.m_didImpact = true;
		result.m_impactPos = startPos;
		result.m_impactDist = 0;
		result.m_impactNormal = fwdNormal * -1.f;
		return result;
	}

	Vec3 toCenter = sphereCenter - startPos;
	float SCiLength = DotProduct3D(toCenter, fwdNormal);
	Vec3 SCi = fwdNormal * SCiLength;
	Vec3 SCjk = toCenter - SCi;
	float SCjkLengthSquared = SCjk.GetLengthSquared();
	if (SCjkLengthSquared >= sphereRadius * sphereRadius)
	{
		result.m_didImpact = false;
		return result;
	}

	result.m_didImpact = true;
	float a = sqrtf(sphereRadius * sphereRadius - SCjkLengthSquared);
	result.m_impactDist = SCiLength - a;
	if (result.m_impactDist >= maxDist || result.m_impactDist <= 0.f)
	{
		result.m_didImpact = false;
		return result;
	}
	result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
	result.m_impactNormal = (startPos - sphereCenter).GetNormalized();
	return result;
}

RaycastResult3D RaycastVsCapsule3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Capsule3 capsule)
{
	RaycastResult3D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	// Is point inside, return
	if (IsPointInsideCapsule3D(startPos, capsule))
	{
		result.m_didImpact = true;
		result.m_impactPos = startPos;
		result.m_impactDist = 0;
		result.m_impactNormal = fwdNormal * -1.f;
		return result;
	}

	LineSegment3 ray(startPos, startPos + fwdNormal * maxDist);


	//if (result.m_didImpact)
	//{
	//	bestCapsule = GetNearestPointOnLineSegment3D(capsule.m_start, capsule.m_end, bestRay);
	//	Vec3 ToBestRayNormal = (bestRay - bestCapsule).GetNormalized();
	//	result.m_impactPos = bestCapsule + ToBestRayNormal * capsule.m_radius;
	//	result.m_impactDist = (result.m_impactPos - startPos).GetLength();
	//	result.m_impactNormal = ToBestRayNormal;
	//}

	return result;
}

RaycastResult3D RaycastVsZCynlinder3D(Vec3 startPos, Vec3 fwdNormal, float maxDist, Vec2 cylCenterXY, FloatRange cylMinMaxZ, float cylRadius)
{
	RaycastResult3D result;
	result.m_rayStartPos = startPos;
	result.m_rayFwdNormal = fwdNormal;
	result.m_rayMaxLength = maxDist;

	// Is point inside, return
	if (IsPointInsideZCylinder3D(startPos, cylCenterXY, cylMinMaxZ, cylRadius))
	{
		result.m_didImpact = true;
		result.m_impactPos = startPos;
		result.m_impactDist = 0;
		result.m_impactNormal = fwdNormal * -1.f;
		return result;
	}

	Vec2 startPosXY = Vec2(startPos.x, startPos.y);
	Vec2 fwdNormalXY = Vec2(fwdNormal.x, fwdNormal.y).GetNormalized();

	Vec2 toDiscCenter = cylCenterXY - startPosXY;
	Vec2& iBasis = fwdNormalXY;
	Vec2 jBasis = iBasis.GetRotated90Degrees();
	float altitude = DotProduct2D(toDiscCenter, jBasis);

	// If too far left or right, return
	if (altitude >= cylRadius)
	{
		result.m_didImpact = false;
		return result;
	}
	if (altitude <= -cylRadius)
	{
		result.m_didImpact = false;
		return result;
	}

	// Come and go in Z
	FloatRange zRange;
	bool zFlip = false;
	Vec3 endPos = startPos + fwdNormal * maxDist;

	if ((endPos.z - startPos.z) == 0)
	{
		zRange.m_min = INFINITY * -1;
		zRange.m_max = INFINITY;
	}
	else
	{
		float oneOverZRange = 1.f / (endPos.z - startPos.z);
		zRange.m_min = (cylMinMaxZ.m_min - startPos.z) * oneOverZRange;
		zRange.m_max = (cylMinMaxZ.m_max - startPos.z) * oneOverZRange;
	}

	if (zRange.m_min > zRange.m_max)
	{
		float temp = zRange.m_min;
		zRange.m_min = zRange.m_max;
		zRange.m_max = temp;
		zFlip = true;
	}

	// Come and go in cylinder's body
	float a = sqrtf((cylRadius * cylRadius) - (altitude * altitude));
	float SCi = DotProduct2D(toDiscCenter, iBasis);
	float impactInXYDist = SCi - a;
	float impactOutXYDist = SCi + a;

	FloatRange tRange;
	float oneOverTRange = 1.f / (Vec2(endPos.x, endPos.y) - Vec2(startPos.x, startPos.y)).GetLength();
	tRange.m_min = impactInXYDist * oneOverTRange;
	tRange.m_max = impactOutXYDist * oneOverTRange;

	if (tRange.IsOverlappingWith(zRange))
	{
		FloatRange OverlapRange;
		bool isHitOnCap = false;

		if (zRange.m_min > tRange.m_min)
		{
			OverlapRange.m_min = zRange.m_min;
			isHitOnCap = true;
		}
		else
		{
			OverlapRange.m_min = tRange.m_min;
		}
		if (zRange.m_max < tRange.m_max)
		{
			OverlapRange.m_max = zRange.m_max;
		}
		else
		{
			OverlapRange.m_max = tRange.m_max;
		}

		result.m_didImpact = true;
		result.m_impactDist = maxDist * OverlapRange.m_min;

		if (result.m_impactDist > maxDist || result.m_impactDist < 0.0f)
		{
			result.m_didImpact = false;
			return result;
		}

		result.m_impactPos = startPos + fwdNormal * result.m_impactDist;
		if (isHitOnCap)
		{
			if (zFlip)
			{
				result.m_impactNormal = Vec3(0.f, 0.f, 1.f);
			}
			else
			{
				result.m_impactNormal = Vec3(0.f, 0.f, -1.f);
			}
		}
		else
		{
			result.m_impactNormal = (result.m_impactPos - Vec3(cylCenterXY.x, cylCenterXY.y, result.m_impactPos.z)).GetNormalized();
		}
	}

	return result;
}

void MouseToRaycast(Camera* camera, Vec2 cursorPosition, Vec3& out_startPos, Vec3& out_forwardDir)
{
	Mat44 cameraMatrix = camera->GetModelMatrix();
	Vec3 transitX = cameraMatrix.GetIBasis3D() * camera->m_zNearPlane;

	float h = camera->m_zNearPlane * tanf(ConvertDegreesToRadians(camera->m_perspectiveFOV) / 2.f) * 2.f;
	float w = Window::GetMainWindowInstance()->GetAspect() * h;

	IntVec2 clientDim = Window::GetMainWindowInstance()->GetClientDimensions();

	float y = RangeMapClamped(cursorPosition.x, 0.f, (float)clientDim.x, -(w / 2), w / 2);
	float z = RangeMapClamped(cursorPosition.y, 0.f, (float)clientDim.y, -(h / 2), h / 2);

	Vec3 transitY = cameraMatrix.GetJBasis3D() * -1.f * y;
	Vec3 transitZ = cameraMatrix.GetKBasis3D() * z;

	Vec3 cursorWorldPos = camera->m_position + transitX + transitY + transitZ;

	out_startPos = camera->m_position;
	out_forwardDir = (cursorWorldPos - camera->m_position).GetNormalized();
}

RaycastResult3D MouseRaycastVsPlane3D(Camera* camera, Vec2 cursorPosition, Plane3 plane)
{
	Vec3 start;
	Vec3 fwd;
	MouseToRaycast(camera, cursorPosition, start, fwd);

	return RaycastVsPlane3D(start, fwd, FLT_MAX, plane);
}

RaycastResult3D MouseRaycastVsSphere3D(Camera* camera, Vec2 cursorPosition, Vec3 center, float radius)
{
	Vec3 start;
	Vec3 fwd;
	MouseToRaycast(camera, cursorPosition, start, fwd);

	return RaycastVsSphere3D(start, fwd, FLT_MAX, center, radius);
}

RaycastResult3D MouseRaycastVsCapsule3D(Camera* camera, Vec2 cursorPosition, Capsule3 capsule)
{
	Vec3 start;
	Vec3 fwd;
	MouseToRaycast(camera, cursorPosition, start, fwd);

	return RaycastVsCapsule3D(start, fwd, FLT_MAX, capsule);
}
