#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"
#include <cmath>

void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float scaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY)
{
	for (int i = 0; i < numVerts; i++)
	{
		Vec3& pos = verts[i].m_position;
		Vec2 iBasis = Vec2(CosDegrees(rotationDegreesAboutZ) * scaleXY, SinDegrees(rotationDegreesAboutZ) * scaleXY);
		Vec2 jBasis = iBasis.GetRotated90Degrees();
		TransformPositionXY3D(pos, iBasis, jBasis, translationXY);
	}
}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Capsule2 const& capsule, Rgba8 const& color)
{
	AddVertsForCapsule2D(verts, capsule.m_start, capsule.m_end, capsule.m_radius, color);
}

void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color)
{
	Vec2 obbCenter = (boneEnd + boneStart) * 0.5f;
	Vec2 obbIBasis = (boneEnd - boneStart).GetNormalized();
	Vec2 obbHalfDimension = Vec2((boneEnd - boneStart).GetLength() * 0.5f, radius);
	OBB2 body = OBB2(obbCenter, obbIBasis, obbHalfDimension);
	AddVertsForOBB2D(verts, body, color);

	constexpr int NUM_BONE_TRIS = 32;
	constexpr int NUM_BONE_VERTS = 3 * NUM_BONE_TRIS;

	float directionOrientationDegrees = obbIBasis.GetOrientationDegrees();
	for (int index = 0; index < NUM_BONE_VERTS; index++)
	{
		float thetaAngle = 360.0f / static_cast<float>(NUM_BONE_TRIS);
		float angle1 = directionOrientationDegrees + (thetaAngle * index);
		float angle2 = directionOrientationDegrees + (thetaAngle * (index + 1));
		if (angle2 <= directionOrientationDegrees + 90.0f)
		{
			Vec2 vert1 = Vec2(radius * CosDegrees(angle1), radius * SinDegrees(angle1));
			Vec2 vert2 = Vec2(radius * CosDegrees(angle2), radius * SinDegrees(angle2));
			vert1 += boneEnd;
			vert2 += boneEnd;
			verts.push_back(Vertex_PCU(Vec3(boneEnd.x, boneEnd.y, 0.0f), color, Vec2(99.0f, 99.0f)));
			verts.push_back(Vertex_PCU(Vec3(vert1.x, vert1.y, 0.0f), color, Vec2(99.0f, 99.0f)));
			verts.push_back(Vertex_PCU(Vec3(vert2.x, vert2.y, 0.0f), color, Vec2(99.0f, 99.0f)));
		}
		if (angle2 > directionOrientationDegrees + 270.0f && angle2 <= directionOrientationDegrees + 360.0f)
		{
			Vec2 vert1 = Vec2(radius * CosDegrees(angle1), radius * SinDegrees(angle1));
			Vec2 vert2 = Vec2(radius * CosDegrees(angle2), radius * SinDegrees(angle2));
			vert1 += boneEnd;
			vert2 += boneEnd;
			verts.push_back(Vertex_PCU(Vec3(boneEnd.x, boneEnd.y, 0.0f), color, Vec2(99.0f, 99.0f)));
			verts.push_back(Vertex_PCU(Vec3(vert1.x, vert1.y, 0.0f), color, Vec2(99.0f, 99.0f)));
			verts.push_back(Vertex_PCU(Vec3(vert2.x, vert2.y, 0.0f), color, Vec2(99.0f, 99.0f)));
		}
		if (angle2 > directionOrientationDegrees + 90.0f && angle2 <= directionOrientationDegrees + 270.0f)
		{
			Vec2 vert1 = Vec2(radius * CosDegrees(angle1), radius * SinDegrees(angle1));
			Vec2 vert2 = Vec2(radius * CosDegrees(angle2), radius * SinDegrees(angle2));
			vert1 += boneStart;
			vert2 += boneStart;
			verts.push_back(Vertex_PCU(Vec3(boneStart.x, boneStart.y, 0.0f), color, Vec2(99.0f, 99.0f)));
			verts.push_back(Vertex_PCU(Vec3(vert1.x, vert1.y, 0.0f), color, Vec2(99.0f, 99.0f)));
			verts.push_back(Vertex_PCU(Vec3(vert2.x, vert2.y, 0.0f), color, Vec2(99.0f, 99.0f)));
		}
	}
}

void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color, int sides)
{
	float degreeStep = 360.f / (float)sides;

	for (int i = 0; i < sides; i++)
	{
		//Angle
		float startDegrees = degreeStep * (float)i;
		float endDegrees = degreeStep * (float)(i + 1);
		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);
		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		Vec3 secondPointPos = Vec3(center.x + radius * cosStart, center.y + radius * sinStart, 0.f);
		Vec3 thirdPointPos = Vec3(center.x + radius * cosEnd, center.y + radius * sinEnd, 0.f);

		Vec2 secondPointUVs = Vec2(cosStart / 2.f + 0.5f, sinStart / 2.f + 0.5f);
		Vec2 thirdPointUVs = Vec2(cosEnd / 2.f + 0.5f, sinEnd / 2.f + 0.5f);

		Vertex_PCU firstVert = Vertex_PCU(Vec3(center.x, center.y, 0.f), color, Vec2(0.5f, 0.5f));
		Vertex_PCU secondVert = Vertex_PCU(secondPointPos, color, secondPointUVs);
		Vertex_PCU thirdVert = Vertex_PCU(thirdPointPos, color, thirdPointUVs);

		verts.push_back(firstVert);
		verts.push_back(secondVert);
		verts.push_back(thirdVert);
	}
}

void AddVertsForDisc2DGradient(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& innerColor, Rgba8 const& outerColor, int sides /*= 32*/)
{
	float degreeStep = 360.f / (float)sides;

	for (int i = 0; i < sides; i++)
	{
		//Angle
		float startDegrees = degreeStep * (float)i;
		float endDegrees = degreeStep * (float)(i + 1);
		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);
		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		Vec3 secondPointPos = Vec3(center.x + radius * cosStart, center.y + radius * sinStart, 0.f);
		Vec3 thirdPointPos = Vec3(center.x + radius * cosEnd, center.y + radius * sinEnd, 0.f);

		Vec2 secondPointUVs = Vec2(cosStart / 2.f + 0.5f, sinStart / 2.f + 0.5f);
		Vec2 thirdPointUVs = Vec2(cosEnd / 2.f + 0.5f, sinEnd / 2.f + 0.5f);

		Vertex_PCU firstVert = Vertex_PCU(Vec3(center.x, center.y, 0.f), innerColor, Vec2(0.5f, 0.5f));
		Vertex_PCU secondVert = Vertex_PCU(secondPointPos, outerColor, secondPointUVs);
		Vertex_PCU thirdVert = Vertex_PCU(thirdPointPos, outerColor, thirdPointUVs);

		verts.push_back(firstVert);
		verts.push_back(secondVert);
		verts.push_back(thirdVert);
	}
}

void AddVertsForRing2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, float thickness, Rgba8 const& color, int sides)
{
	float degressStep = 360.f / (float)sides;
	float innerRadius = radius - 0.5f * thickness;
	float outerRadius = radius + 0.5f * thickness;

	for (int i = 0; i < sides; i++)
	{
		//Angle
		float startDegrees = degressStep * (float)i;
		float endDegrees = degressStep * (float)(i + 1);
		float cosStart = CosDegrees(startDegrees);
		float sinStart = SinDegrees(startDegrees);
		float cosEnd = CosDegrees(endDegrees);
		float sinEnd = SinDegrees(endDegrees);

		//Inner and outer position
		Vec3 innerStartPos = Vec3(center.x + innerRadius * cosStart, center.y + innerRadius * sinStart, 0.f);
		Vec3 outerStartPos = Vec3(center.x + outerRadius * cosStart, center.y + outerRadius * sinStart, 0.f);
		Vec3 outerEndPos = Vec3(center.x + outerRadius * cosEnd, center.y + outerRadius * sinEnd, 0.f);
		Vec3 innerEndPos = Vec3(center.x + innerRadius * cosEnd, center.y + innerRadius * sinEnd, 0.f);

		//Trapezoid is made of 2 triangles: ABC and DEF 

		Vertex_PCU vertA = Vertex_PCU(innerEndPos, color, Vec2::ZERO);
		Vertex_PCU vertB = Vertex_PCU(innerStartPos, color, Vec2::ZERO);
		Vertex_PCU vertC = Vertex_PCU(outerStartPos, color, Vec2::ZERO);
		Vertex_PCU vertF = Vertex_PCU(outerEndPos, color, Vec2::ZERO);

		verts.push_back(vertA);
		verts.push_back(vertB);
		verts.push_back(vertC);

		verts.push_back(vertA);
		verts.push_back(vertC);
		verts.push_back(vertF);
	}
}

void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, Vec2 uvMins, Vec2 uvMaxs)
{
	Vertex_PCU topLeft = Vertex_PCU(Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0), color, Vec2(uvMins.x, uvMaxs.y));
	Vertex_PCU topRight = Vertex_PCU(Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0), color, Vec2(uvMaxs.x, uvMaxs.y));
	Vertex_PCU bottomLeft = Vertex_PCU(Vec3(bounds.m_mins.x, bounds.m_mins.y, 0), color, Vec2(uvMins.x, uvMins.y));
	Vertex_PCU bottomRight = Vertex_PCU(Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0), color, Vec2(uvMaxs.x, uvMins.y));

	verts.push_back(bottomLeft);
	verts.push_back(bottomRight);
	verts.push_back(topRight);

	verts.push_back(topRight);
	verts.push_back(topLeft);
	verts.push_back(bottomLeft);
}

void AddVertsForAABB2DOutline(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005*/)
{
	Vec2 topLeft = Vec2(bounds.m_mins.x, bounds.m_maxs.y);
	Vec2 topRight = Vec2(bounds.m_maxs.x, bounds.m_maxs.y);
	Vec2 bottomLeft = Vec2(bounds.m_mins.x, bounds.m_mins.y);
	Vec2 bottomRight = Vec2(bounds.m_maxs.x, bounds.m_mins.y);

	AddVertsForLineSegment2D(verts, LineSegment2(topLeft, topRight), lineThickness, color);
	AddVertsForLineSegment2D(verts, LineSegment2(topLeft, bottomLeft), lineThickness, color);
	AddVertsForLineSegment2D(verts, LineSegment2(bottomRight, topRight), lineThickness, color);
	AddVertsForLineSegment2D(verts, LineSegment2(bottomRight, bottomLeft), lineThickness, color);
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& orientedBox, Rgba8 const& color)
{
	Vec2 cornerPosOBB2[4];
	orientedBox.GetCornerPoints(cornerPosOBB2);

	Vec3 topRightPos = Vec3(cornerPosOBB2[0].x, cornerPosOBB2[0].y, 0);
	Vec3 topLefttPos = Vec3(cornerPosOBB2[1].x, cornerPosOBB2[1].y, 0);
	Vec3 bottomLeftPos = Vec3(cornerPosOBB2[2].x, cornerPosOBB2[2].y, 0);
	Vec3 bottomRightPos = Vec3(cornerPosOBB2[3].x, cornerPosOBB2[3].y, 0);

	//TODO: Ask how to map UV
	Vertex_PCU topRight = Vertex_PCU(topRightPos, color, Vec2(1, 1));
	Vertex_PCU topLeft = Vertex_PCU(topLefttPos, color, Vec2(0, 1));
	Vertex_PCU bottomLeft = Vertex_PCU(bottomLeftPos, color, Vec2(0, 0));
	Vertex_PCU bottomRight = Vertex_PCU(bottomRightPos, color, Vec2(1, 0));

	verts.push_back(bottomLeft);
	verts.push_back(bottomRight);
	verts.push_back(topRight);

	verts.push_back(topRight);
	verts.push_back(topLeft);
	verts.push_back(bottomLeft);
}

void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, Vec2 const& ibasis, Vec2 const& halfDimension, Rgba8 const& color)
{
	OBB2 obbBox(center, ibasis, halfDimension);
	AddVertsForOBB2D(verts, obbBox, color);
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color)
{
	float radius = thickness * 0.5f;

	Vec2 vecForward = radius * (end - start).GetNormalized();
	Vec2 vecLeft = vecForward.GetRotated90Degrees();
	Vec2 vecRight = vecForward.GetRotatedMinus90Degrees();

	Vec3 fowardLeftPos = Vec3(end.x + vecForward.x + vecLeft.x, end.y + vecForward.y + vecLeft.y, 0.f);
	Vec3 fowardRightPos = Vec3(end.x + vecForward.x + vecRight.x, end.y + vecForward.y + vecRight.y, 0.f);
	Vec3 backLeftPos = Vec3(start.x - vecForward.x + vecLeft.x, start.y - vecForward.y + vecLeft.y, 0.f);
	Vec3 backRightPos = Vec3(start.x - vecForward.x + vecRight.x, start.y - vecForward.y + vecRight.y, 0.f);

	Vertex_PCU forwardLeft = Vertex_PCU(fowardLeftPos, color, Vec2(0, 1));
	Vertex_PCU forwardRight = Vertex_PCU(fowardRightPos, color, Vec2(1, 1));
	Vertex_PCU backLeft = Vertex_PCU(backLeftPos, color, Vec2(0, 0));
	Vertex_PCU backRight = Vertex_PCU(backRightPos, color, Vec2(1, 0));

	verts.push_back(backLeft);
	verts.push_back(backRight);
	verts.push_back(forwardRight);

	verts.push_back(forwardRight);
	verts.push_back(forwardLeft);
	verts.push_back(backLeft);
}

void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, LineSegment2 const& lineSegment, float thickness, Rgba8 const& color)
{
	AddVertsForLineSegment2D(verts, lineSegment.m_start, lineSegment.m_end, thickness, color);
}

void AddVertsForLineSegment2DWith2Colors(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color1, Rgba8 const& color2)
{
	float radius = thickness * 0.5f;

	Vec2 vecForward = radius * (end - start).GetNormalized();
	Vec2 vecLeft = vecForward.GetRotated90Degrees();
	Vec2 vecRight = vecForward.GetRotatedMinus90Degrees();

	Vec3 fowardLeftPos = Vec3(end.x + vecForward.x + vecLeft.x, end.y + vecForward.y + vecLeft.y, 0.f);
	Vec3 fowardRightPos = Vec3(end.x + vecForward.x + vecRight.x, end.y + vecForward.y + vecRight.y, 0.f);
	Vec3 backLeftPos = Vec3(start.x - vecForward.x + vecLeft.x, start.y - vecForward.y + vecLeft.y, 0.f);
	Vec3 backRightPos = Vec3(start.x - vecForward.x + vecRight.x, start.y - vecForward.y + vecRight.y, 0.f);

	Vertex_PCU forwardLeft = Vertex_PCU(fowardLeftPos, color2, Vec2(0, 1));
	Vertex_PCU forwardRight = Vertex_PCU(fowardRightPos, color2, Vec2(1, 1));
	Vertex_PCU backLeft1 = Vertex_PCU(backLeftPos, color1, Vec2(0, 0));

	Vertex_PCU backLeft = Vertex_PCU(backLeftPos, color1, Vec2(0, 0));
	Vertex_PCU backRight = Vertex_PCU(backRightPos, color1, Vec2(1, 0));
	Vertex_PCU forwardRight1 = Vertex_PCU(fowardRightPos, color2, Vec2(1, 1));

	verts.push_back(forwardLeft);
	verts.push_back(forwardRight);
	verts.push_back(backLeft1);

	verts.push_back(forwardRight1);
	verts.push_back(backLeft);
	verts.push_back(backRight);
}

void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 const& color)
{
	Vec2 iBasis = (tipPos - tailPos).GetNormalized();
	Vec2 jBasis = iBasis.GetRotated90Degrees();
	Vec2 leftPoint = tipPos - iBasis * arrowSize - jBasis * arrowSize;
	Vec2 rightPoint = tipPos - iBasis * arrowSize + jBasis * arrowSize;
	AddVertsForLineSegment2D(verts, tailPos, tipPos, lineThickness, color);
	AddVertsForLineSegment2D(verts, tipPos, leftPoint, lineThickness, color);
	AddVertsForLineSegment2D(verts, tipPos, rightPoint, lineThickness, color);
}

void AddVertsForWireframeAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color, float lineThickness)
{
	Vec3 topLeft = Vec3(bounds.m_mins.x, bounds.m_maxs.y, 0);
	Vec3 topRight = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, 0);
	Vec3 bottomLeft = Vec3(bounds.m_mins.x, bounds.m_mins.y, 0);
	Vec3 bottomRight = Vec3(bounds.m_maxs.x, bounds.m_mins.y, 0);

	AddVertsForCylinder3DNoCap(verts, bottomLeft, topLeft, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, bottomLeft, bottomRight, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, topRight, topLeft, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, topRight, bottomRight, lineThickness, color);
}

void AddVertsForConvexPoly2D(std::vector<Vertex_PCU>& verts, ConvexPoly2 const& convex, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/)
{
	Vertex_PCU p0 = Vertex_PCU(Vec3(convex.GetPositions()[0], 0.f), color, Vec2::ZERO);

	for (size_t i = 1; i < convex.GetSize() - 1; i++)
	{
		Vertex_PCU p1;
		Vertex_PCU p2;

		p1 = Vertex_PCU(Vec3(convex.GetPositions()[i], 0.f), color, Vec2::ZERO);

		if (i == convex.GetSize() - 1)
		{
			p2 = Vertex_PCU(Vec3(convex.GetPositions()[0], 0.f), color, Vec2::ZERO);
		}
		else
		{
			p2 = Vertex_PCU(Vec3(convex.GetPositions()[i + 1], 0.f), color, Vec2::ZERO);
		}

		verts.push_back(p0);
		verts.push_back(p1);
		verts.push_back(p2);
	}
}

void AddVertsForConvexPoly2DPlanes(std::vector<Vertex_PCU>& verts, ConvexHull2 const& convex, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005f*/)
{
	for (size_t i = 0; i < convex.GetSize(); i++)
	{
		AddVertsForPlane2D(verts, convex.GetPlanes()[i], color, lineThickness);
	}
}

void AddVertsForConvexPoly2DOutline(std::vector<Vertex_PCU>& verts, ConvexPoly2 const& convex, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005f*/)
{
	for (size_t i = 0; i < convex.GetSize(); i++)
	{
		if (i == convex.GetSize() - 1)
		{
			AddVertsForLineSegment2D(verts, convex.GetPositions()[i], convex.GetPositions()[0], lineThickness, color);
		}
		else
		{
			AddVertsForLineSegment2D(verts, convex.GetPositions()[i], convex.GetPositions()[i + 1], lineThickness, color);
		}
	}
}

void AddVertsForPlane2D(std::vector<Vertex_PCU>& verts, Plane2 const& plane2, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005f*/)
{
	Vec2 Q = plane2.m_normal * plane2.m_distanceFromOrigin;

	Vec2 left = Q + plane2.m_normal.GetRotated90Degrees() * 9999.f;
	Vec2 right = Q + plane2.m_normal.GetRotatedMinus90Degrees() * 9999.f;

	AddVertsForLineSegment2D(verts, left, right, lineThickness, color);
	//AddVertsForArrow2D(verts, Q, Q + plane2.m_normal * 5.f, lineThickness, lineThickness, color);
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PCU TR = Vertex_PCU(topRight, color, UVs.m_maxs);
	Vertex_PCU TL = Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y));
	Vertex_PCU BR = Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y));
	Vertex_PCU BL = Vertex_PCU(bottomLeft, color, UVs.m_mins);

	verts.push_back(BL);
	verts.push_back(BR);
	verts.push_back(TR);

	verts.push_back(TR);
	verts.push_back(TL);
	verts.push_back(BL);
}

void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color, const AABB2& UVs)
{
	Vertex_PCU TR = Vertex_PCU(topRight, color, UVs.m_maxs);
	Vertex_PCU TL = Vertex_PCU(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y));
	Vertex_PCU BR = Vertex_PCU(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y));
	Vertex_PCU BL = Vertex_PCU(bottomLeft, color, UVs.m_mins);

	int startIndex = (int)verts.size();

	verts.push_back(BL);
	verts.push_back(BR);
	verts.push_back(TR);
	verts.push_back(TL);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 normal = CrossProduct3D((bottomRight - bottomLeft), (topLeft - bottomLeft)).GetNormalized();

	// Calculate tangent and bitangent
	Vec3 edge1 = bottomRight - bottomLeft;
	Vec3 edge2 = topLeft - bottomLeft;
	Vec2 deltaUV1 = Vec2(UVs.m_maxs.x, UVs.m_mins.y) - UVs.m_mins;
	Vec2 deltaUV2 = Vec2(UVs.m_mins.x, UVs.m_maxs.y) - UVs.m_mins;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	Vec3 tangent, bitangent;

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = tangent.GetNormalized();

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = bitangent.GetNormalized();

	Vertex_PCUTBN TR = Vertex_PCUTBN(topRight, color, UVs.m_maxs, tangent, bitangent, normal);
	Vertex_PCUTBN TL = Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), tangent, bitangent, normal);
	Vertex_PCUTBN BR = Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), tangent, bitangent, normal);
	Vertex_PCUTBN BL = Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, tangent, bitangent, normal);

	int startIndex = (int)verts.size();

	verts.push_back(BL);
	verts.push_back(BR);
	verts.push_back(TR);
	verts.push_back(TL);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);
}

void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 normal = CrossProduct3D((bottomRight - bottomLeft), (topLeft - bottomLeft)).GetNormalized();

	Vec3 edge1 = bottomRight - bottomLeft;
	Vec3 edge2 = topLeft - bottomLeft;
	Vec2 deltaUV1 = Vec2(UVs.m_maxs.x, UVs.m_mins.y) - UVs.m_mins;
	Vec2 deltaUV2 = Vec2(UVs.m_mins.x, UVs.m_maxs.y) - UVs.m_mins;

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	Vec3 tangent, bitangent;

	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = tangent.GetNormalized();

	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = bitangent.GetNormalized();

	Vertex_PCUTBN TR = Vertex_PCUTBN(topRight, color, UVs.m_maxs, tangent, bitangent, normal);
	Vertex_PCUTBN TL = Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), tangent, bitangent, normal);
	Vertex_PCUTBN BR = Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), tangent, bitangent, normal);
	Vertex_PCUTBN BL = Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, tangent, bitangent, normal);

	verts.push_back(BL);
	verts.push_back(BR);
	verts.push_back(TR);

	verts.push_back(TR);
	verts.push_back(TL);
	verts.push_back(BL);
}

void AddVertsForQuad3DInverseNormal(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 normal = CrossProduct3D((bottomRight - bottomLeft), (topLeft - bottomLeft)).GetNormalized() * -1;

	Vertex_PCUTBN TR = Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3::ZERO, Vec3::ZERO, normal);
	Vertex_PCUTBN TL = Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3::ZERO, Vec3::ZERO, normal);
	Vertex_PCUTBN BR = Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3::ZERO, Vec3::ZERO, normal);
	Vertex_PCUTBN BL = Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3::ZERO, Vec3::ZERO, normal);

	int startIndex = (int)verts.size();

	verts.push_back(BL);
	verts.push_back(BR);
	verts.push_back(TR);
	verts.push_back(TL);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);
}

void AddVertsForQuad3DNoNormal(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vertex_PCUTBN TR = Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3::ZERO, Vec3::ZERO, Vec3::ZERO);
	Vertex_PCUTBN TL = Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3::ZERO, Vec3::ZERO, Vec3::ZERO);
	Vertex_PCUTBN BR = Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3::ZERO, Vec3::ZERO, Vec3::ZERO);
	Vertex_PCUTBN BL = Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3::ZERO, Vec3::ZERO, Vec3::ZERO);

	int startIndex = (int)verts.size();

	verts.push_back(BL);
	verts.push_back(BR);
	verts.push_back(TR);
	verts.push_back(TL);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);
}

void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 middleTopPos = Vec3(topLeft.x, topLeft.y + (topRight.y - topLeft.y) * 0.5f, topLeft.z);
	Vec3 middleBottomPos = Vec3(bottomLeft.x, bottomLeft.y + (bottomRight.y - bottomLeft.y) * 0.5f, bottomLeft.z);

	Vec3 normalMidTop = CrossProduct3D((topLeft - middleTopPos), (middleBottomPos - middleTopPos)).GetNormalized();
	Vec3 normalMidTBottom = CrossProduct3D((bottomRight - middleBottomPos), (middleTopPos - middleBottomPos)).GetNormalized();

	Vertex_PCUTBN TR = Vertex_PCUTBN(topRight, color, UVs.m_maxs, Vec3::ZERO, Vec3::ZERO, (topRight - topLeft).GetNormalized());
	Vertex_PCUTBN TL = Vertex_PCUTBN(topLeft, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y), Vec3::ZERO, Vec3::ZERO, (topLeft - topRight).GetNormalized());
	Vertex_PCUTBN MT = Vertex_PCUTBN(middleTopPos, color, Vec2(UVs.m_mins.x + (UVs.m_maxs.x - UVs.m_mins.x) * 0.5f, UVs.m_maxs.y), Vec3::ZERO, Vec3::ZERO, normalMidTop);
	Vertex_PCUTBN MB = Vertex_PCUTBN(middleBottomPos, color, Vec2(UVs.m_mins.x + (UVs.m_maxs.x - UVs.m_mins.x) * 0.5f, UVs.m_mins.y), Vec3::ZERO, Vec3::ZERO, normalMidTBottom);
	Vertex_PCUTBN BR = Vertex_PCUTBN(bottomRight, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y), Vec3::ZERO, Vec3::ZERO, (bottomRight - bottomLeft).GetNormalized());
	Vertex_PCUTBN BL = Vertex_PCUTBN(bottomLeft, color, UVs.m_mins, Vec3::ZERO, Vec3::ZERO, (bottomLeft - bottomRight).GetNormalized());

	int startIndex = (int)verts.size();

	verts.push_back(BL);
	verts.push_back(MB);
	verts.push_back(BR);
	verts.push_back(TR);
	verts.push_back(MT);
	verts.push_back(TL);

	indexes.push_back(startIndex);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 4);

	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex);

	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);

	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 1);
}

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color, AABB2 const& UVs)
{
	Vec3 BLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 BRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 TLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 TRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);

	Vec3 BLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 BRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 TLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 TRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);

	AddVertsForQuad3D(verts, BRFar, BLFar, TRFar, TLFar, color, UVs); // X
	AddVertsForQuad3D(verts, BLNear, BRNear, TLNear, TRNear, color, UVs); // -X
	AddVertsForQuad3D(verts, BRNear, BRFar, TRNear, TRFar, color, UVs); // Y
	AddVertsForQuad3D(verts, BLFar, BLNear, TLFar, TLNear, color, UVs); // -Y
	AddVertsForQuad3D(verts, TLNear, TRNear, TLFar, TRFar, color, UVs); // Z
	AddVertsForQuad3D(verts, BRNear, BLNear, BRFar, BLFar, color, UVs); // -Z
}

void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, AABB3 const& bounds, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 BLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 BRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 TLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 TRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);

	Vec3 BLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 BRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 TLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 TRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);

	Vertex_PCU BLN = Vertex_PCU(BLNear, color, UVs.m_mins);
	Vertex_PCU BRN = Vertex_PCU(BRNear, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y));
	Vertex_PCU TLN = Vertex_PCU(TLNear, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y));
	Vertex_PCU TRN = Vertex_PCU(TRNear, color, UVs.m_maxs);

	Vertex_PCU BLF = Vertex_PCU(BLFar, color, UVs.m_mins);
	Vertex_PCU BRF = Vertex_PCU(BRFar, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y));
	Vertex_PCU TLF = Vertex_PCU(TLFar, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y));
	Vertex_PCU TRF = Vertex_PCU(TRFar, color, UVs.m_maxs);

	int startIndex = (int)verts.size();

	verts.push_back(BLN);	//0
	verts.push_back(BRN);	//1
	verts.push_back(TRN);	//2
	verts.push_back(TLN);	//3
	verts.push_back(BLF);	//4
	verts.push_back(BRF);	//5
	verts.push_back(TRF);	//6
	verts.push_back(TLF);	//7

	// FRONT
	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);

	//BACK
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 7);
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 7);
	indexes.push_back(startIndex + 6);

	//TOP
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 7);

	//BOTTOM
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 0);

	//LEFT
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 7);

	//RIGHT
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 2);
}

void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, AABB3 const& bounds, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 BLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 BRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 TLNear = Vec3(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 TRNear = Vec3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);

	Vec3 BLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 BRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 TLFar = Vec3(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 TRFar = Vec3(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);

	AddVertsForQuad3D(verts, indexes, BLNear, BRNear, TLNear, TRNear, color, UVs);
	AddVertsForQuad3D(verts, indexes, BLFar, BLNear, TLFar, TLNear, color, UVs);
	AddVertsForQuad3D(verts, indexes, BLFar, BRFar, BLNear, BRNear, color, UVs);
	AddVertsForQuad3D(verts, indexes, BRFar, BLFar, TRFar, TLFar, color, UVs);
	AddVertsForQuad3D(verts, indexes, BRNear, BRFar, TRNear, TRFar, color, UVs);
	AddVertsForQuad3D(verts, indexes, TLNear, TRNear, TLFar, TRFar, color, UVs);
}

void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& bounds, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 allPoint[8];
	bounds.GetCornerPoints(allPoint);

	Vec3 BLNear = allPoint[0];
	Vec3 BRNear = allPoint[1];
	Vec3 TLNear = allPoint[2];
	Vec3 TRNear = allPoint[3];

	Vec3 BLFar = allPoint[4];
	Vec3 BRFar = allPoint[5];
	Vec3 TLFar = allPoint[6];
	Vec3 TRFar = allPoint[7];

	AddVertsForQuad3D(verts, BRFar, BLFar, TRFar, TLFar, color, UVs); // X
	AddVertsForQuad3D(verts, BLNear, BRNear, TLNear, TRNear, color, UVs); // -X
	AddVertsForQuad3D(verts, BRNear, BRFar, TRNear, TRFar, color, UVs); // Y
	AddVertsForQuad3D(verts, BLFar, BLNear, TLFar, TLNear, color, UVs); // -Y
	AddVertsForQuad3D(verts, TLNear, TRNear, TLFar, TRFar, color, UVs); // Z
	AddVertsForQuad3D(verts, BRNear, BLNear, BRFar, BLFar, color, UVs); // -Z
}

void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, OBB3 const& bounds, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 allPoint[8];
	bounds.GetCornerPoints(allPoint);

	Vec3 BLNear = allPoint[0];
	Vec3 BRNear = allPoint[1];
	Vec3 TLNear = allPoint[2];
	Vec3 TRNear = allPoint[3];

	Vec3 BLFar = allPoint[4];
	Vec3 BRFar = allPoint[5];
	Vec3 TLFar = allPoint[6];
	Vec3 TRFar = allPoint[7];

	Vertex_PCU BLN = Vertex_PCU(BLNear, color, UVs.m_mins);
	Vertex_PCU BRN = Vertex_PCU(BRNear, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y));
	Vertex_PCU TLN = Vertex_PCU(TLNear, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y));
	Vertex_PCU TRN = Vertex_PCU(TRNear, color, UVs.m_maxs);

	Vertex_PCU BLF = Vertex_PCU(BLFar, color, UVs.m_mins);
	Vertex_PCU BRF = Vertex_PCU(BRFar, color, Vec2(UVs.m_maxs.x, UVs.m_mins.y));
	Vertex_PCU TLF = Vertex_PCU(TLFar, color, Vec2(UVs.m_mins.x, UVs.m_maxs.y));
	Vertex_PCU TRF = Vertex_PCU(TRFar, color, UVs.m_maxs);

	int startIndex = (int)verts.size();

	verts.push_back(BLN);	//0
	verts.push_back(BRN);	//1
	verts.push_back(TRN);	//2
	verts.push_back(TLN);	//3
	verts.push_back(BLF);	//4
	verts.push_back(BRF);	//5
	verts.push_back(TRF);	//6
	verts.push_back(TLF);	//7

	// FRONT
	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 3);

	//BACK
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 7);
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 7);
	indexes.push_back(startIndex + 6);

	//TOP
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 2);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 7);

	//BOTTOM
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 0);

	//LEFT
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 0);
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 4);
	indexes.push_back(startIndex + 3);
	indexes.push_back(startIndex + 7);

	//RIGHT
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 5);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 1);
	indexes.push_back(startIndex + 6);
	indexes.push_back(startIndex + 2);
}

void AddVertsForOBB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, OBB3 const& bounds, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/)
{
	Vec3 allPoint[8];
	bounds.GetCornerPoints(allPoint);

	Vec3 BLNear = allPoint[0];
	Vec3 BRNear = allPoint[1];
	Vec3 TLNear = allPoint[2];
	Vec3 TRNear = allPoint[3];

	Vec3 BLFar = allPoint[4];
	Vec3 BRFar = allPoint[5];
	Vec3 TLFar = allPoint[6];
	Vec3 TRFar = allPoint[7];

	AddVertsForQuad3D(verts, indexes, BLNear, BRNear, TLNear, TRNear, color, UVs);
	AddVertsForQuad3D(verts, indexes, BLFar, BLNear, TLFar, TLNear, color, UVs);
	AddVertsForQuad3D(verts, indexes, BLFar, BRFar, BLNear, BRNear, color, UVs);
	AddVertsForQuad3D(verts, indexes, BRFar, BLFar, TRFar, TLFar, color, UVs);
	AddVertsForQuad3D(verts, indexes, BRNear, BRFar, TRNear, TRFar, color, UVs);
	AddVertsForQuad3D(verts, indexes, TLNear, TRNear, TLFar, TRFar, color, UVs);
}

void AddVertsForSphere(std::vector<Vertex_PCU>& verts, const Vec3& center, float radius, Rgba8 const& color, AABB2 const& UVs, int numLatitudeSlices, int numLongtitudeSlices)
{
	float latStep = 180.f / numLatitudeSlices;
	float lonStep = 360.f / numLongtitudeSlices;

	float uvHeightStep = 1.f / (float)numLatitudeSlices;
	float uvWidthStep = 1.f / (float)numLongtitudeSlices;

	for (int i = 0; i < numLatitudeSlices; i++)
	{
		for (int j = 0; j < numLongtitudeSlices; j++)
		{
			Vec3 BL = center + Vec3::MakeFromPolarDegrees(90.f - latStep * i, lonStep * j, radius);
			Vec3 BR = center + Vec3::MakeFromPolarDegrees(90.f - latStep * i, lonStep * (j + 1), radius);
			Vec3 TL = center + Vec3::MakeFromPolarDegrees(90.f - latStep * (i + 1), lonStep * j, radius);
			Vec3 TR = center + Vec3::MakeFromPolarDegrees(90.f - latStep * (i + 1), lonStep * (j + 1), radius);

			Vec2 uvMin = Vec2(UVs.m_mins.x + uvWidthStep * j, UVs.m_mins.y + uvHeightStep * i);
			Vec2 uvMax = Vec2(UVs.m_mins.x + uvWidthStep * (j + 1), UVs.m_mins.y + uvHeightStep * (i + 1));

			AddVertsForQuad3D(verts, BL, BR, TL, TR, color, AABB2(uvMin, uvMax));
		}
	}
}

void AddVertsForSphere(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& center, float radius, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/, int numStacks /*= 32*/, int numSlices /*= 64*/)
{
	int startIndex = (unsigned int)verts.size();

	float stackStep = 180.f / numStacks;
	float sliceStep = 360.f / numSlices;

	float uvHeightStep = 1.f / (float)numStacks;
	float uvWidthStep = 1.f / (float)numSlices;

	verts.push_back(Vertex_PCUTBN(center + Vec3(0, 0, -radius), color, Vec2(0.5f, 1.0f), Vec3(), Vec3(), Vec3(0, 0, -1)));

	for (int stack = 1; stack < numStacks; ++stack)
	{
		for (int slice = 0; slice <= numSlices; ++slice)
		{
			Vec3 pos = center + Vec3::MakeFromPolarDegrees(90.f - stackStep * stack, sliceStep * slice, radius);
			Vec2 uv = Vec2(UVs.m_mins.x + uvWidthStep * slice, UVs.m_maxs.y - uvHeightStep * stack);
			Vec3 n = (pos - center).GetNormalized();

			verts.push_back(Vertex_PCUTBN(pos, color, uv, Vec3(), Vec3(), n));
		}
	}

	verts.push_back(Vertex_PCUTBN(center + Vec3(0, 0, radius), color, Vec2(0.5f, 0.0f), Vec3(), Vec3(), Vec3(0, 0, 1)));

	for (int slice = 0; slice < numSlices; ++slice)
	{
		indexes.push_back(startIndex);
		indexes.push_back(startIndex + slice + 2);
		indexes.push_back(startIndex + slice + 1);
	}

	// Middle stacks
	for (int stack = 1; stack < numStacks - 1; ++stack)
	{
		for (int slice = 0; slice < numSlices; ++slice)
		{
			int BL = startIndex + (stack - 1) * (numSlices + 1) + slice + 1;
			int BR = BL + 1;
			int TL = BL + numSlices + 1;
			int TR = BL + 1 + numSlices + 1;

			indexes.push_back(BL);
			indexes.push_back(BR);
			indexes.push_back(TR);

			indexes.push_back(BL);
			indexes.push_back(TR);
			indexes.push_back(TL);
		}
	}
	//Top cap
	int topPoleIndex = (unsigned int)verts.size() - 1;
	int topStart = (numStacks - 2) * (numSlices + 1);
	for (int slice = 0; slice < numSlices; ++slice)
	{
		indexes.push_back(startIndex + topPoleIndex);
		indexes.push_back(startIndex + topStart + slice + 1);
		indexes.push_back(startIndex + topStart + slice + 2);
	}

	CalculateTangentSpaceBasisVectors(verts, indexes);
}

void AddVertsForSphere(std::vector<Vertex_PCUTBN>& verts, const Vec3& center, float radius, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/, int numLatitudeSlices /*= 32*/, int numLongtitudeSlices /*= 64*/)
{
	float latStep = 180.f / numLatitudeSlices;
	float lonStep = 360.f / numLongtitudeSlices;

	float uvHeightStep = 1.f / (float)numLatitudeSlices;
	float uvWidthStep = 1.f / (float)numLongtitudeSlices;

	for (int i = 0; i < numLatitudeSlices; i++)
	{
		for (int j = 0; j < numLongtitudeSlices; j++)
		{
			Vec3 BL = center + Vec3::MakeFromPolarDegrees(90.f - latStep * i, lonStep * j, radius);
			Vec3 BR = center + Vec3::MakeFromPolarDegrees(90.f - latStep * i, lonStep * (j + 1), radius);
			Vec3 TL = center + Vec3::MakeFromPolarDegrees(90.f - latStep * (i + 1), lonStep * j, radius);
			Vec3 TR = center + Vec3::MakeFromPolarDegrees(90.f - latStep * (i + 1), lonStep * (j + 1), radius);

			Vec2 uvMin = Vec2(UVs.m_mins.x + uvWidthStep * j, UVs.m_mins.y + uvHeightStep * i);
			Vec2 uvMax = Vec2(UVs.m_mins.x + uvWidthStep * (j + 1), UVs.m_mins.y + uvHeightStep * (i + 1));

			AddVertsForQuad3D(verts, BL, BR, TL, TR, color, AABB2(uvMin, uvMax));
		}
	}
}

void AddVertsForSphere(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& center, float radius, Rgba8 const& color /*= Rgba8::COLOR_WHITE*/, AABB2 const& UVs /*= AABB2::ZERO_TO_ONE*/, int numStacks /*= 32*/, int numSlices /*= 64*/)
{
	int startIndex = (unsigned int)verts.size();

	float stackStep = 180.f / numStacks;
	float sliceStep = 360.f / numSlices;

	float uvHeightStep = 1.f / (float)numStacks;
	float uvWidthStep = 1.f / (float)numSlices;

	verts.push_back(Vertex_PCU(center + Vec3(0, 0, -radius), color, Vec2(0.5f, 1.0f)));

	for (int stack = 1; stack < numStacks; ++stack)
	{
		for (int slice = 0; slice <= numSlices; ++slice)
		{
			Vec3 pos = center + Vec3::MakeFromPolarDegrees(90.f - stackStep * stack, sliceStep * slice, radius);
			Vec2 uv = Vec2(UVs.m_mins.x + uvWidthStep * slice, UVs.m_maxs.y - uvHeightStep * stack);

			verts.push_back(Vertex_PCU(pos, color, uv));
		}
	}

	verts.push_back(Vertex_PCU(center + Vec3(0, 0, radius), color, Vec2(0.5f, 0.0f)));

	for (int slice = 0; slice < numSlices; ++slice)
	{
		indexes.push_back(startIndex);
		indexes.push_back(startIndex + slice + 2);
		indexes.push_back(startIndex + slice + 1);
	}

	// Middle stacks
	for (int stack = 1; stack < numStacks - 1; ++stack)
	{
		for (int slice = 0; slice < numSlices; ++slice)
		{
			int BL = startIndex + (stack - 1) * (numSlices + 1) + slice + 1;
			int BR = BL + 1;
			int TL = BL + numSlices + 1;
			int TR = BL + 1 + numSlices + 1;

			indexes.push_back(BL);
			indexes.push_back(BR);
			indexes.push_back(TR);

			indexes.push_back(BL);
			indexes.push_back(TR);
			indexes.push_back(TL);
		}
	}
	//Top cap
	int topPoleIndex = (unsigned int)verts.size() - 1;
	int topStart = (numStacks - 2) * (numSlices + 1);
	for (int slice = 0; slice < numSlices; ++slice)
	{
		indexes.push_back(startIndex + topPoleIndex);
		indexes.push_back(startIndex + topStart + slice + 1);
		indexes.push_back(startIndex + topStart + slice + 2);
	}
}

void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, const Mat44& transform)
{
	for (int i = 0; i < (int)verts.size(); i++)
	{
		verts[i].m_position = transform.TransformPosition3D(verts[i].m_position);
	}
}

AABB2 GetVertexBounds2D(const std::vector<Vertex_PCU>& verts)
{
	float minX = verts[0].m_position.x;
	float minY = verts[0].m_position.y;
	float maxX = verts[0].m_position.x;
	float maxY = verts[0].m_position.y;

	for (int i = 0; i < (int)verts.size(); i++)
	{
		if (verts[i].m_position.x < minX)
		{
			minX = verts[i].m_position.x;
		}
		if (verts[i].m_position.y < minY)
		{
			minY = verts[i].m_position.y;
		}
		if (verts[i].m_position.x > maxX)
		{
			maxX = verts[i].m_position.x;
		}
		if (verts[i].m_position.y > maxY)
		{
			maxY = verts[i].m_position.y;
		}
	}

	return AABB2(minX, minY, maxX, maxY);
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	for (int i = 0; i < numSlices; i++)
	{
		Vertex_PCU vertStart = Vertex_PCU(start, color, UVcenter);

		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 startP1Pos = start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vertex_PCU vertSP1 = Vertex_PCU(startP1Pos, color, Vec2(startP1XY.x / (2 * radius) + UVcenter.x, startP1XY.y / (2 * radius) + UVcenter.y));

		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 startP2Pos = start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vertex_PCU vertSP2 = Vertex_PCU(startP2Pos, color, Vec2(startP2XY.x / (2 * radius) + UVcenter.x, startP2XY.y / (2 * radius) + UVcenter.y));

		verts.push_back(vertStart);
		verts.push_back(vertSP2);
		verts.push_back(vertSP1);

		Vertex_PCU vertEnd = Vertex_PCU(end, color, Vec2(0.5f, 0.5f));

		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 endP1Pos = end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vertex_PCU vertEP1 = Vertex_PCU(endP1Pos, color, Vec2((endP1XY.x) / (2 * radius) + UVcenter.x, (endP1XY.y) / (2 * radius) + UVcenter.y));

		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 endP2Pos = end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vertex_PCU vertEP2 = Vertex_PCU(endP2Pos, color, Vec2((endP2XY.x) / (2 * radius) + UVcenter.x, (endP2XY.y) / (2 * radius) + UVcenter.y));

		verts.push_back(vertEnd);
		verts.push_back(vertEP1);
		verts.push_back(vertEP2);

		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	unsigned int topCenterIndex = (int)verts.size();

	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Vertex_PCU vertBottom = Vertex_PCU(start, color, UVcenter);
	Vertex_PCU vertTop = Vertex_PCU(end, color, UVcenter);

	verts.push_back(vertTop);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 posXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 pos = end + iBasis * posXY.x + jBasis * posXY.y;
		verts.push_back(Vertex_PCU(pos, color, Vec2(posXY.x / (2 * radius) + UVcenter.x, posXY.y / (2 * radius) + UVcenter.y)));
	}
	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(topCenterIndex);
		indexes.push_back(topCenterIndex + 1 + i);
		indexes.push_back(topCenterIndex + 1 + next);
	}

	unsigned int bottomCenterIndex = (int)verts.size();
	verts.push_back(vertBottom);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 posXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 pos = start + iBasis * posXY.x + jBasis * posXY.y;
		verts.push_back(Vertex_PCU(pos, color, Vec2(posXY.x / (2 * radius) + UVcenter.x, posXY.y / (2 * radius) + UVcenter.y)));
	}

	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(bottomCenterIndex);
		indexes.push_back(bottomCenterIndex + 1 + next);
		indexes.push_back(bottomCenterIndex + 1 + i);
	}

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 startP1Pos = start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 startP2Pos = start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 endP1Pos = end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 endP2Pos = end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, indexes, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForCylinder3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Vertex_PCUTBN vertBottom = Vertex_PCUTBN(start, color, UVcenter, Vec3::ZERO, Vec3::ZERO, kBasis * -1.f);
	Vertex_PCUTBN vertTop = Vertex_PCUTBN(end, color, UVcenter, Vec3::ZERO, Vec3::ZERO, kBasis);

	unsigned int topCenterIndex = (int)verts.size();
	verts.push_back(vertTop);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 topPosXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 topPos = end + iBasis * topPosXY.x + jBasis * topPosXY.y;
		verts.push_back(Vertex_PCUTBN(topPos, color, Vec2(topPosXY.x / (2 * radius) + UVcenter.x, topPosXY.y / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, kBasis));
	}
	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(topCenterIndex);
		indexes.push_back(topCenterIndex + 1 + i);
		indexes.push_back(topCenterIndex + 1 + next);
	}

	unsigned int bottomCenterIndex = (int)verts.size();
	verts.push_back(vertBottom);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 bottomPosXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 bottomPos = start + iBasis * bottomPosXY.x + jBasis * bottomPosXY.y;
		verts.push_back(Vertex_PCUTBN(bottomPos, color, Vec2(bottomPosXY.x / (2 * radius) + UVcenter.x, bottomPosXY.y / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, kBasis * -1.f));
	}

	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(bottomCenterIndex);
		indexes.push_back(bottomCenterIndex + 1 + next);
		indexes.push_back(bottomCenterIndex + 1 + i);
	}

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 startP1Pos = start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 startP2Pos = start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 endP1Pos = end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 endP2Pos = end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, indexes, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForCylinder3DNoCap(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 startP1Pos = start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 startP2Pos = start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 endP1Pos = end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 endP2Pos = end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, indexes, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForCylinder3DNoCap(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 startP1Pos = start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 startP2Pos = start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 endP1Pos = end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 endP2Pos = end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForCapsule3D(std::vector<Vertex_PCU>& verts, Capsule3 capsule, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	if (capsule.m_end == capsule.m_start)
	{
		AddVertsForSphere(verts, capsule.m_start, capsule.m_radius, color, UVs, numSlices, numSlices * 2);
		return;
	}

	Vec3 axis = capsule.m_end - capsule.m_start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Mat44 startHemisphereTransform;
	startHemisphereTransform.SetIJKT3D(iBasis, jBasis, kBasis, capsule.m_start);

	AddVertsForHemisphere3D(verts, startHemisphereTransform, capsule.m_radius, color, UVs, numSlices);

	Mat44 endHemisphereTransform = startHemisphereTransform;
	endHemisphereTransform.AppendXRotation(180);
	endHemisphereTransform.SetTranslation3D(capsule.m_end);

	AddVertsForHemisphere3D(verts, endHemisphereTransform, capsule.m_radius, color, UVs, numSlices);


	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 startP1Pos = capsule.m_start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 startP2Pos = capsule.m_start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 endP1Pos = capsule.m_end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 endP2Pos = capsule.m_end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForCapsule3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Capsule3 capsule, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 32*/)
{
	Vec3 axis = capsule.m_end - capsule.m_start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Mat44 startHemisphereTransform;
	startHemisphereTransform.SetIJKT3D(iBasis, jBasis, kBasis, capsule.m_start);

	AddVertsForHemisphere3D(verts, indexes, startHemisphereTransform, capsule.m_radius, color, UVs, numSlices);

	Mat44 endHemisphereTransform = startHemisphereTransform;
	endHemisphereTransform.AppendXRotation(180);
	endHemisphereTransform.SetTranslation3D(capsule.m_end);

	AddVertsForHemisphere3D(verts, indexes, endHemisphereTransform, capsule.m_radius, color, UVs, numSlices);


	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 startP1Pos = capsule.m_start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 startP2Pos = capsule.m_start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 endP1Pos = capsule.m_end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 endP2Pos = capsule.m_end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, indexes, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForCapsule3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Capsule3 capsule, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 32*/)
{
	Vec3 axis = capsule.m_end - capsule.m_start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Mat44 startHemisphereTransform;
	startHemisphereTransform.SetIJKT3D(iBasis, jBasis, kBasis, capsule.m_start);

	AddVertsForHemisphere3D(verts, indexes, startHemisphereTransform, capsule.m_radius, color, UVs, numSlices);

	Mat44 endHemisphereTransform = startHemisphereTransform;
	endHemisphereTransform.AppendXRotation(180);
	endHemisphereTransform.SetTranslation3D(capsule.m_end);

	AddVertsForHemisphere3D(verts, indexes, endHemisphereTransform, capsule.m_radius, color, UVs, numSlices);

	int cylSlides = numSlices * 64;
	float stepAngle = 360.f / (float)cylSlides;
	float uvsStep = 1.f / cylSlides;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	for (int i = 0; i < cylSlides; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 startP1Pos = capsule.m_start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 startP2Pos = capsule.m_start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 endP1Pos = capsule.m_end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 endP2Pos = capsule.m_end + iBasis * endP2XY.x + jBasis * endP2XY.y;
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, indexes, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForHemisphere3D(std::vector<Vertex_PCU>& verts, Mat44 transform, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	float latStart = 0.f;
	float latEnd = 90.f;

	int latlices = numSlices / 2;
	int lonSlices = numSlices;

	for (int lat = 0; lat < latlices; lat++)
	{
		for (int lon = 0; lon < lonSlices * 2; lon++)
		{
			float lat1 = RangeMapClamped((float)lat, 0.f, (float)latlices, latStart, latEnd);
			float lon1 = RangeMapClamped((float)lon, 0.f, (float)lonSlices, 0.f, 360.f);

			float lat2 = RangeMapClamped((float)(lat + 1), 0.f, (float)latlices, latStart, latEnd);
			float lon2 = RangeMapClamped((float)(lon + 1), 0.f, (float)lonSlices, 0.f, 360.f);

			Vec3 TL = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat1, lon1, radius));
			Vec3 TR = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat1, lon2, radius));
			Vec3 BR = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat2, lon2, radius));
			Vec3 BL = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat2, lon1, radius));

			float uMin = RangeMapClamped(lon1, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
			float uMax = RangeMapClamped(lon2, 0.f, 360.f, UVs.m_mins.x, UVs.m_maxs.x);
			float vMin = RangeMapClamped(lat1, latStart, latEnd, UVs.m_mins.y, UVs.m_maxs.y);
			float vMax = RangeMapClamped(lat2, latStart, latEnd, UVs.m_mins.y, UVs.m_maxs.y);

			AABB2 uvBox = AABB2(Vec2(uMin, vMin), Vec2(uMax, vMax));

			AddVertsForQuad3D(verts, BL, BR, TL, TR, color, uvBox);
		}
	}
}

void AddVertsForHemisphere3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Mat44 transform, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 32*/)
{
	int startIndex = (unsigned int)verts.size();

	int numStacks = numSlices / 2;

	float stackStep = 180.f / numStacks;
	float sliceStep = 360.f / numSlices;

	float uvHeightStep = 1.f / (float)numStacks;
	float uvWidthStep = 1.f / (float)numSlices;

	verts.push_back(Vertex_PCU(transform.TransformPosition3D(Vec3(0, 0, -radius)), color, Vec2(0.5f, 1.0f)));

	for (int stack = 1; stack < numStacks / 2; ++stack)
	{
		for (int slice = 0; slice <= numSlices; ++slice)
		{
			Vec3 pos = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(90.f - stackStep * stack, sliceStep * slice, radius));
			Vec2 uv = Vec2(UVs.m_mins.x + uvWidthStep * slice, UVs.m_maxs.y - uvHeightStep * stack);

			verts.push_back(Vertex_PCU(pos, color, uv));
		}
	}

	for (int slice = 0; slice < numSlices; ++slice)
	{
		indexes.push_back(startIndex);
		indexes.push_back(startIndex + slice + 2);
		indexes.push_back(startIndex + slice + 1);
	}

	// Middle stacks
	for (int stack = 1; stack < numStacks / 2 - 1; ++stack)
	{
		for (int slice = 0; slice < numSlices; ++slice)
		{
			int BL = startIndex + (stack - 1) * (numSlices + 1) + slice + 1;
			int BR = BL + 1;
			int TL = BL + numSlices + 1;
			int TR = BL + 1 + numSlices + 1;

			indexes.push_back(BL);
			indexes.push_back(BR);
			indexes.push_back(TR);

			indexes.push_back(BL);
			indexes.push_back(TR);
			indexes.push_back(TL);
		}
	}
}

void AddVertsForHemisphere3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Mat44 transform, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 32*/)
{
	int startIndex = (unsigned int)verts.size();

	int numStacks = (numSlices / 2);

	float stackStep = 180.f / numStacks;
	float sliceStep = 360.f / numSlices;

	float uvHeightStep = 1.f / (float)numStacks;
	float uvWidthStep = 1.f / (float)numSlices;

	Vec3 center = transform.GetTranslation3D();

	verts.push_back(Vertex_PCUTBN(center + Vec3(0, 0, -radius), color, Vec2(0.5f, 1.0f), Vec3(), Vec3(), Vec3(0, 0, -1)));

	for (int stack = 1; stack <= numStacks / 2; ++stack)
	{
		for (int slice = 0; slice <= numSlices; ++slice)
		{
			Vec3 pos = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(90.f - stackStep * stack, sliceStep * slice, radius));
			Vec2 uv = Vec2(UVs.m_mins.x + uvWidthStep * slice, UVs.m_maxs.y - uvHeightStep * stack);
			Vec3 n = (pos - center).GetNormalized();

			verts.push_back(Vertex_PCUTBN(pos, color, uv, Vec3(), Vec3(), n));
		}
	}

	for (int slice = 0; slice < numSlices; ++slice)
	{
		indexes.push_back(startIndex);
		indexes.push_back(startIndex + slice + 2);
		indexes.push_back(startIndex + slice + 1);
	}

	// Middle stacks
	for (int stack = 1; stack < numStacks / 2; ++stack)
	{
		for (int slice = 0; slice < numSlices; ++slice)
		{
			int BL = startIndex + (stack - 1) * (numSlices + 1) + slice + 1;
			int BR = BL + 1;
			int TL = BL + numSlices + 1;
			int TR = BL + 1 + numSlices + 1;

			indexes.push_back(BL);
			indexes.push_back(BR);
			indexes.push_back(TR);

			indexes.push_back(BL);
			indexes.push_back(TR);
			indexes.push_back(TL);
		}
	}

	CalculateTangentSpaceBasisVectors(verts, indexes);
}

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color, const AABB2& UVs, int numSlices)
{
	float stepAngle = 360.f / (float)numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	for (int i = 0; i < numSlices; i++)
	{

		Vertex_PCU vertEnd = Vertex_PCU(end, color, UVcenter);
		Vertex_PCU vertStart = Vertex_PCU(start, color, UVcenter);

		Vec2 baseP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 baseP1Pos = start + iBasis * baseP1XY.x + jBasis * baseP1XY.y;
		Vertex_PCU vertBaseP1 = Vertex_PCU(baseP1Pos, color, Vec2((baseP1XY.x) / (2 * radius) + UVcenter.x, (baseP1XY.y) / (2 * radius) + UVcenter.y));

		Vec2 baseP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 baseP2Pos = start + iBasis * baseP2XY.x + jBasis * baseP2XY.y;
		Vertex_PCU vertBaseP2 = Vertex_PCU(baseP2Pos, color, Vec2((baseP2XY.x) / (2 * radius) + UVcenter.x, (baseP2XY.y) / (2 * radius) + UVcenter.y));

		verts.push_back(vertStart);
		verts.push_back(vertBaseP2);
		verts.push_back(vertBaseP1);


		verts.push_back(vertEnd);
		verts.push_back(vertBaseP1);
		verts.push_back(vertBaseP2);
	}
}

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	float stepAngle = 360.f / (float)numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Vertex_PCU vertBottom = Vertex_PCU(start, color, UVcenter);
	Vertex_PCU vertTop = Vertex_PCU(end, color, UVcenter);

	unsigned int bottomCenterIndex = (int)verts.size();
	verts.push_back(vertBottom);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 posXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 pos = start + iBasis * posXY.x + jBasis * posXY.y;
		verts.push_back(Vertex_PCU(pos, color, Vec2(posXY.x / (2 * radius) + UVcenter.x, posXY.y / (2 * radius) + UVcenter.y)));
	}

	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(bottomCenterIndex);
		indexes.push_back(bottomCenterIndex + 1 + next);
		indexes.push_back(bottomCenterIndex + 1 + i);
	}

	unsigned int topCenterIndex = (int)verts.size();
	verts.push_back(vertTop);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 posXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 pos = start + iBasis * posXY.x + jBasis * posXY.y;
		verts.push_back(Vertex_PCU(pos, color, Vec2(posXY.x / (2 * radius) + UVcenter.x, posXY.y / (2 * radius) + UVcenter.y)));
	}

	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(topCenterIndex);
		indexes.push_back(topCenterIndex + 1 + i);
		indexes.push_back(topCenterIndex + 1 + next);
	}
}

void AddVertsForCone3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& start, const Vec3& end, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	float stepAngle = 360.f / (float)numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Vertex_PCUTBN vertBottom = Vertex_PCUTBN(start, color, UVcenter, Vec3::ZERO, Vec3::ZERO, kBasis * -1.f);
	Vertex_PCUTBN vertTop = Vertex_PCUTBN(end, color, UVcenter, Vec3::ZERO, Vec3::ZERO, kBasis);

	unsigned int bottomCenterIndex = (int)verts.size();
	verts.push_back(vertBottom);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 posXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 pos = start + iBasis * posXY.x + jBasis * posXY.y;
		verts.push_back(Vertex_PCUTBN(pos, color, Vec2(posXY.x / (2 * radius) + UVcenter.x, posXY.y / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, kBasis * -1.f));
	}

	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(bottomCenterIndex);
		indexes.push_back(bottomCenterIndex + 1 + next);
		indexes.push_back(bottomCenterIndex + 1 + i);
	}

	unsigned int topCenterIndex = (int)verts.size();
	verts.push_back(vertTop);

	for (int i = 0; i < numSlices; ++i)
	{
		Vec2 posXY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 pos = start + iBasis * posXY.x + jBasis * posXY.y;
		verts.push_back(Vertex_PCUTBN(pos, color, Vec2(posXY.x / (2 * radius) + UVcenter.x, posXY.y / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, iBasis * posXY.x + jBasis * posXY.y));
	}

	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(topCenterIndex);
		indexes.push_back(topCenterIndex + 1 + i);
		indexes.push_back(topCenterIndex + 1 + next);
	}
}

void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, const AABB2& UVs /*= AABB2::ZERO_TO_ONE*/, int numSlices /*= 8*/)
{
	float arrowHeadLength = radius * 3.0f;
	float arrowHeadRadius = radius * 2.0f;
	Vec3 dir = (end - start).GetNormalized();

	AddVertsForCylinder3D(verts, start, end - dir * arrowHeadLength, radius, color, UVs, numSlices);
	AddVertsForCone3D(verts, end - dir * arrowHeadLength, end, arrowHeadRadius, color, UVs, numSlices);
}

void AddVertsForZCylinder3D(std::vector<Vertex_PCU>& verts, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, int numSlices, const Rgba8& color, const AABB2& UVs)
{
	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 bottom = Vec3(centerXY.x, centerXY.y, minMaxZ.m_min);
	Vec3 top = Vec3(centerXY.x, centerXY.y, minMaxZ.m_max);

	for (int i = 0; i < numSlices; i++)
	{
		Vertex_PCU vertBottom = Vertex_PCU(bottom, color, UVcenter);

		Vec2 bottom1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 bottom1Pos = Vec3(bottom.x + bottom1XY.x, bottom.y + bottom1XY.y, bottom.z);
		Vertex_PCU vertBottom1 = Vertex_PCU(bottom1Pos, color, Vec2(bottom1XY.x / (2 * radius) + UVcenter.x, bottom1XY.y / (2 * radius) + UVcenter.y));

		Vec2 bottom2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 bottom2Pos = Vec3(bottom.x + bottom2XY.x, bottom.y + bottom2XY.y, bottom.z);
		Vertex_PCU vertBottom2 = Vertex_PCU(bottom2Pos, color, Vec2(bottom2XY.x / (2 * radius) + UVcenter.x, bottom2XY.y / (2 * radius) + UVcenter.y));

		verts.push_back(vertBottom);
		verts.push_back(vertBottom2);
		verts.push_back(vertBottom1);

		Vertex_PCU vertTop = Vertex_PCU(top, color, UVcenter);

		Vec2 top1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 top1Pos = Vec3(top.x + top1XY.x, top.y + top1XY.y, top.z);
		Vertex_PCU vertTop1 = Vertex_PCU(top1Pos, color, Vec2((top1XY.x) / (2 * radius) + UVcenter.x, (top1XY.y) / (2 * radius) + UVcenter.y));

		Vec2 top2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 top2Pos = Vec3(top.x + top2XY.x, top.y + top2XY.y, top.z);
		Vertex_PCU vertTop2 = Vertex_PCU(top2Pos, color, Vec2((top2XY.x) / (2 * radius) + UVcenter.x, (top2XY.y) / (2 * radius) + UVcenter.y));

		verts.push_back(vertTop);
		verts.push_back(vertTop1);
		verts.push_back(vertTop2);

		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, bottom1Pos, bottom2Pos, top1Pos, top2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}


}

void AddVertsForZCylinder3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, int numSlices, const Rgba8& color, const AABB2& UVs)
{
	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 bottom = Vec3(centerXY.x, centerXY.y, minMaxZ.m_min);
	Vec3 top = Vec3(centerXY.x, centerXY.y, minMaxZ.m_max);

	Vertex_PCU vertBottom = Vertex_PCU(bottom, color, UVcenter);
	Vertex_PCU vertTop = Vertex_PCU(top, color, UVcenter);

	unsigned int topCenterIndex = (int)verts.size();
	verts.push_back(vertTop);

	for (int i = 0; i < numSlices; ++i)
	{
		float angle = stepAngle * i;
		float x = centerXY.x + radius * CosDegrees(angle);
		float y = centerXY.y + radius * SinDegrees(angle);
		Vec3 topVertex = Vec3(x, y, minMaxZ.m_max);
		verts.push_back(Vertex_PCU(topVertex, color, UVs.GetUVForPoint(Vec2(x, y))));
	}
	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(topCenterIndex);
		indexes.push_back(topCenterIndex + 1 + i);
		indexes.push_back(topCenterIndex + 1 + next);
	}

	unsigned int bottomCenterIndex = (int)verts.size();
	verts.push_back(vertBottom);

	for (int i = 0; i < numSlices; ++i)
	{
		float angle = stepAngle * i;
		float x = centerXY.x + radius * CosDegrees(angle);
		float y = centerXY.y + radius * SinDegrees(angle);
		Vec3 bottomVertex = Vec3(x, y, minMaxZ.m_min);
		verts.push_back(Vertex_PCU(bottomVertex, color, UVs.GetUVForPoint(Vec2(x, y))));
	}

	for (int i = 0; i < numSlices; ++i)
	{
		unsigned int next = (i + 1) % numSlices;

		indexes.push_back(bottomCenterIndex);
		indexes.push_back(bottomCenterIndex + 1 + next);
		indexes.push_back(bottomCenterIndex + 1 + i);
	}

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 bottom1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 bottom1Pos = Vec3(bottom.x + bottom1XY.x, bottom.y + bottom1XY.y, bottom.z);
		Vec2 bottom2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 bottom2Pos = Vec3(bottom.x + bottom2XY.x, bottom.y + bottom2XY.y, bottom.z);
		Vec2 top1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 top1Pos = Vec3(top.x + top1XY.x, top.y + top1XY.y, top.z);
		Vec2 top2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 top2Pos = Vec3(top.x + top2XY.x, top.y + top2XY.y, top.z);
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, indexes, bottom1Pos, bottom2Pos, top1Pos, top2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForZCylinder3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, int numSlices, const Rgba8& color, const AABB2& UVs)
{
	int startBottomIndex = (int)indexes.size();
	float stepAngle = 360.f / (float)numSlices;
	float uvsStep = 1.f / numSlices;
	Vec2 UVcenter = (UVs.m_maxs - UVs.m_mins) / 2.f;

	Vec3 bottom = Vec3(centerXY.x, centerXY.y, minMaxZ.m_min);
	Vec3 top = Vec3(centerXY.x, centerXY.y, minMaxZ.m_max);

	Vertex_PCUTBN vertBottom = Vertex_PCUTBN(bottom, color, UVcenter, Vec3::ZERO, Vec3::ZERO, Vec3(0.f, 0.f, -1.f));
	Vertex_PCUTBN vertTop = Vertex_PCUTBN(top, color, UVcenter, Vec3::ZERO, Vec3::ZERO, Vec3(0.f, 0.f, 1.f));

	verts.push_back(vertBottom);
	verts.push_back(vertTop);

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 bottom1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 bottom1Pos = Vec3(bottom.x + bottom1XY.x, bottom.y + bottom1XY.y, bottom.z);
		Vertex_PCUTBN vertBottom1 = Vertex_PCUTBN(bottom1Pos, color, Vec2(bottom1XY.x / (2 * radius) + UVcenter.x, bottom1XY.y / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, Vec3(0.f, 0.f, -1.f));

		Vec2 bottom2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 bottom2Pos = Vec3(bottom.x + bottom2XY.x, bottom.y + bottom2XY.y, bottom.z);
		Vertex_PCUTBN vertBottom2 = Vertex_PCUTBN(bottom2Pos, color, Vec2(bottom2XY.x / (2 * radius) + UVcenter.x, bottom2XY.y / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, Vec3(0.f, 0.f, -1.f));

		verts.push_back(vertBottom2);
		verts.push_back(vertBottom1);

		indexes.push_back(startBottomIndex + 0);
		indexes.push_back(startBottomIndex + 2 + i);
		indexes.push_back(startBottomIndex + 2 + i + 1);
	}

	int startTopIndex = (int)indexes.size();

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 top1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 top1Pos = Vec3(top.x + top1XY.x, top.y + top1XY.y, top.z);
		Vertex_PCUTBN vertTop1 = Vertex_PCUTBN(top1Pos, color, Vec2((top1XY.x) / (2 * radius) + UVcenter.x, (top1XY.y) / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, Vec3(0.f, 0.f, 1.f));

		Vec2 top2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 top2Pos = Vec3(top.x + top2XY.x, top.y + top2XY.y, top.z);
		Vertex_PCUTBN vertTop2 = Vertex_PCUTBN(top2Pos, color, Vec2((top2XY.x) / (2 * radius) + UVcenter.x, (top2XY.y) / (2 * radius) + UVcenter.y), Vec3::ZERO, Vec3::ZERO, Vec3(0.f, 0.f, 1.f));
		;
		verts.push_back(vertTop1);
		verts.push_back(vertTop2);

		indexes.push_back(startTopIndex + 0);
		indexes.push_back(startTopIndex + 2 + i);
		indexes.push_back(startTopIndex + 2 + i + 1);
	}

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 bottom1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 bottom1Pos = Vec3(bottom.x + bottom1XY.x, bottom.y + bottom1XY.y, bottom.z);
		Vec2 bottom2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 bottom2Pos = Vec3(bottom.x + bottom2XY.x, bottom.y + bottom2XY.y, bottom.z);
		Vec2 top1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 top1Pos = Vec3(top.x + top1XY.x, top.y + top1XY.y, top.z);
		Vec2 top2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 top2Pos = Vec3(top.x + top2XY.x, top.y + top2XY.y, top.z);
		Vec2 uvMinQuad = Vec2(UVs.m_mins.x + uvsStep * i, 0);
		Vec2 uvMaxQuad = Vec2(UVs.m_mins.x + uvsStep * (i + 1), 1);

		AddVertsForQuad3D(verts, indexes, bottom1Pos, bottom2Pos, top1Pos, top2Pos, color, AABB2(uvMinQuad, uvMaxQuad));
	}
}

void AddVertsForWireframeQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005f*/, int numSlices)
{
	AddVertsForCylinder3DNoCap(verts, bottomLeft, bottomRight, lineThickness, color, AABB2::ZERO_TO_ONE, numSlices);
	AddVertsForCylinder3DNoCap(verts, bottomLeft, topLeft, lineThickness, color, AABB2::ZERO_TO_ONE, numSlices);
	AddVertsForCylinder3DNoCap(verts, topRight, bottomRight, lineThickness, color, AABB2::ZERO_TO_ONE, numSlices);
	AddVertsForCylinder3DNoCap(verts, topRight, topLeft, lineThickness, color, AABB2::ZERO_TO_ONE, numSlices);
}

void AddVertsForWireframeAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& box, const Rgba8& color, float lineThickness)
{
	Vec3 min = box.m_mins;
	Vec3 max = box.m_maxs;
	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, min.y, min.z), Vec3(max.x, min.y, min.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, max.y, min.z), Vec3(max.x, max.y, min.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, min.y, min.z), Vec3(min.x, max.y, min.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(max.x, min.y, min.z), Vec3(max.x, max.y, min.z), lineThickness, color);

	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, min.y, max.z), Vec3(max.x, min.y, max.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, max.y, max.z), Vec3(max.x, max.y, max.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, min.y, max.z), Vec3(min.x, max.y, max.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(max.x, min.y, max.z), Vec3(max.x, max.y, max.z), lineThickness, color);

	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, min.y, min.z), Vec3(min.x, min.y, max.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(max.x, min.y, min.z), Vec3(max.x, min.y, max.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(min.x, max.y, min.z), Vec3(min.x, max.y, max.z), lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, Vec3(max.x, max.y, min.z), Vec3(max.x, max.y, max.z), lineThickness, color);
}

void AddVertsForWireframeOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& box, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005f*/)
{
	Vec3 allPoint[8];
	box.GetCornerPoints(allPoint);

	Vec3 BLNear = allPoint[0];
	Vec3 BRNear = allPoint[1];
	Vec3 TLNear = allPoint[2];
	Vec3 TRNear = allPoint[3];

	Vec3 BLFar = allPoint[4];
	Vec3 BRFar = allPoint[5];
	Vec3 TLFar = allPoint[6];
	Vec3 TRFar = allPoint[7];

	AddVertsForCylinder3DNoCap(verts, BLNear, BRNear, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, BLNear, TLNear, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, TRNear, BRNear, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, TRNear, TLNear, lineThickness, color);

	AddVertsForCylinder3DNoCap(verts, BLFar, BRFar, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, BLFar, TLFar, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, TRFar, BRFar, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, TRFar, TLFar, lineThickness, color);

	AddVertsForCylinder3DNoCap(verts, BLNear, BLFar, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, BRNear, BRFar, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, TLNear, TLFar, lineThickness, color);
	AddVertsForCylinder3DNoCap(verts, TRNear, TRFar, lineThickness, color);
}

void AddVertsForWireframeSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, int numSlices, int numStacks, const Rgba8& color, float lineThickness)
{
	float latStep = 180.f / (float)numStacks;
	float lonStep = 360.f / (float)numSlices;

	for (int i = 0; i < numStacks; i++)
	{
		for (int j = 0; j < numSlices; j++)
		{
			Vec3 BL = center + Vec3::MakeFromPolarDegrees(90.f - latStep * i, lonStep * j, radius);
			Vec3 BR = center + Vec3::MakeFromPolarDegrees(90.f - latStep * i, lonStep * (j + 1), radius);
			Vec3 TL = center + Vec3::MakeFromPolarDegrees(90.f - latStep * (i + 1), lonStep * j, radius);
			Vec3 TR = center + Vec3::MakeFromPolarDegrees(90.f - latStep * (i + 1), lonStep * (j + 1), radius);

			AddVertsForCylinder3DNoCap(verts, BL, BR, lineThickness, color);
			AddVertsForCylinder3DNoCap(verts, BL, TL, lineThickness, color);
			AddVertsForCylinder3DNoCap(verts, TR, TL, lineThickness, color);
			AddVertsForCylinder3DNoCap(verts, TR, BR, lineThickness, color);
		}
	}
}

void AddVertsForWireframeZCylinder3D(std::vector<Vertex_PCU>& verts, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, float numSlices, const Rgba8& color, float lineThickness)
{
	float stepAngle = 360.f / (float)numSlices;
	Vec3 bottom = Vec3(centerXY.x, centerXY.y, minMaxZ.m_min);
	Vec3 top = Vec3(centerXY.x, centerXY.y, minMaxZ.m_max);

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 bottom1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 bottom1Pos = Vec3(bottom.x + bottom1XY.x, bottom.y + bottom1XY.y, bottom.z);
		Vec2 bottom2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 bottom2Pos = Vec3(bottom.x + bottom2XY.x, bottom.y + bottom2XY.y, bottom.z);

		AddVertsForCylinder3DNoCap(verts, bottom, bottom1Pos, lineThickness, color);
		AddVertsForCylinder3DNoCap(verts, bottom, bottom2Pos, lineThickness, color);
		AddVertsForCylinder3DNoCap(verts, bottom1Pos, bottom2Pos, lineThickness, color);

		Vec2 top1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 top1Pos = Vec3(top.x + top1XY.x, top.y + top1XY.y, top.z);
		Vec2 top2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 top2Pos = Vec3(top.x + top2XY.x, top.y + top2XY.y, top.z);

		AddVertsForCylinder3DNoCap(verts, top, top1Pos, lineThickness, color);
		AddVertsForCylinder3DNoCap(verts, top, top2Pos, lineThickness, color);
		AddVertsForCylinder3DNoCap(verts, top1Pos, top2Pos, lineThickness, color);


		AddVertsForCylinder3DNoCap(verts, bottom1Pos, bottom2Pos, lineThickness, color);
		AddVertsForCylinder3DNoCap(verts, bottom1Pos, top1Pos, lineThickness, color);
		AddVertsForCylinder3DNoCap(verts, top2Pos, top1Pos, lineThickness, color);
		AddVertsForCylinder3DNoCap(verts, top2Pos, bottom2Pos, lineThickness, color);
	}
}

void AddVertsForWireframeHemisphere3D(std::vector<Vertex_PCU>& verts, Mat44 transform, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, float lineThickness, int numSlices /*= 32*/)
{
	float latStart = 0.f;
	float latEnd = 90.f;

	int latlices = numSlices / 2;
	int lonSlices = numSlices;

	for (int lat = 0; lat < latlices; lat++)
	{
		for (int lon = 0; lon < lonSlices * 2; lon++)
		{
			float lat1 = RangeMapClamped((float)lat, 0.f, (float)latlices, latStart, latEnd);
			float lon1 = RangeMapClamped((float)lon, 0.f, (float)lonSlices, 0.f, 360.f);

			float lat2 = RangeMapClamped((float)(lat + 1), 0.f, (float)latlices, latStart, latEnd);
			float lon2 = RangeMapClamped((float)(lon + 1), 0.f, (float)lonSlices, 0.f, 360.f);

			Vec3 TL = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat1, lon1, radius));
			Vec3 TR = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat1, lon2, radius));
			Vec3 BR = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat2, lon2, radius));
			Vec3 BL = transform.TransformPosition3D(Vec3::MakeFromPolarDegrees(lat2, lon1, radius));

			AddVertsForWireframeQuad3D(verts, BL, BR, TL, TR, color, lineThickness, numSlices);
		}
	}
}

void AddVertsForWireframeCapsule3D(std::vector<Vertex_PCU>& verts, Capsule3 capsule, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005f*/, int numSlices /*= 32*/)
{
	Vec3 axis = capsule.m_end - capsule.m_start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	Mat44 startHemisphereTransform;
	startHemisphereTransform.SetIJKT3D(iBasis, jBasis, kBasis, capsule.m_start);

	AddVertsForWireframeHemisphere3D(verts, startHemisphereTransform, capsule.m_radius, color, lineThickness, numSlices);

	Mat44 endHemisphereTransform = startHemisphereTransform;
	endHemisphereTransform.AppendXRotation(180);
	endHemisphereTransform.SetTranslation3D(capsule.m_end);

	AddVertsForWireframeHemisphere3D(verts, endHemisphereTransform, capsule.m_radius, color, lineThickness, numSlices);


	float stepAngle = 360.f / (float)numSlices;

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 startP1Pos = capsule.m_start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 startP2Pos = capsule.m_start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, capsule.m_radius);
		Vec3 endP1Pos = capsule.m_end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), capsule.m_radius);
		Vec3 endP2Pos = capsule.m_end + iBasis * endP2XY.x + jBasis * endP2XY.y;

		AddVertsForWireframeQuad3D(verts, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, lineThickness, numSlices);
	}
}

void AddVertsForWireframeCylinder3DNoCap(std::vector<Vertex_PCU>& verts, Vec3 start, Vec3 end, float radius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/, float lineThickness /*= 0.005f*/, int numSlices /*= 32*/)
{
	Vec3 axis = end - start;
	Vec3 kBasis = axis.GetNormalized();
	Vec3 iBasis;
	Vec3 jBasis;

	if (fabs(DotProduct3D(kBasis, Vec3(1.f, 0.f, 0.f))) < 0.999f)
	{
		jBasis = CrossProduct3D(Vec3(1.f, 0.f, 0.f), kBasis).GetNormalized();
		iBasis = CrossProduct3D(jBasis, kBasis);
	}
	else
	{
		iBasis = CrossProduct3D(kBasis, Vec3(0.f, 1.f, 0.f)).GetNormalized();
		jBasis = CrossProduct3D(kBasis, iBasis);
	}

	float stepAngle = 360.f / (float)numSlices;

	for (int i = 0; i < numSlices; i++)
	{
		Vec2 startP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 startP1Pos = start + iBasis * startP1XY.x + jBasis * startP1XY.y;
		Vec2 startP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 startP2Pos = start + iBasis * startP2XY.x + jBasis * startP2XY.y;
		Vec2 endP1XY = Vec2::MakeFromPolarDegrees(stepAngle * i, radius);
		Vec3 endP1Pos = end + iBasis * endP1XY.x + jBasis * endP1XY.y;
		Vec2 endP2XY = Vec2::MakeFromPolarDegrees(stepAngle * (i + 1), radius);
		Vec3 endP2Pos = end + iBasis * endP2XY.x + jBasis * endP2XY.y;

		AddVertsForWireframeQuad3D(verts, startP1Pos, startP2Pos, endP1Pos, endP2Pos, color, lineThickness, numSlices);
	}
}

void AddVertsForLineAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& box, const Rgba8& color /*= Rgba8::COLOR_WHITE*/)
{
	Vec3 halfDim = box.GetHalfDimension();
	Vec3 center = box.GetCenter();

	Vertex_PCU BLN = Vertex_PCU(Vec3(center + Vec3(-halfDim.x, -halfDim.y, -halfDim.z)), color);
	Vertex_PCU BRN = Vertex_PCU(Vec3(center + Vec3(halfDim.x, -halfDim.y, -halfDim.z)), color);
	Vertex_PCU BLF = Vertex_PCU(Vec3(center + Vec3(-halfDim.x, halfDim.y, -halfDim.z)), color);
	Vertex_PCU BRF = Vertex_PCU(Vec3(center + Vec3(halfDim.x, halfDim.y, -halfDim.z)), color);

	Vertex_PCU TLN = Vertex_PCU(Vec3(center + Vec3(-halfDim.x, -halfDim.y, halfDim.z)), color);
	Vertex_PCU TRN = Vertex_PCU(Vec3(center + Vec3(halfDim.x, -halfDim.y, halfDim.z)), color);
	Vertex_PCU TLF = Vertex_PCU(Vec3(center + Vec3(-halfDim.x, halfDim.y, halfDim.z)), color);
	Vertex_PCU TRF = Vertex_PCU(Vec3(center + Vec3(halfDim.x, halfDim.y, halfDim.z)), color);

	verts.push_back(BLN);
	verts.push_back(BRN);

	verts.push_back(BLN);
	verts.push_back(BLF);

	verts.push_back(BLN);
	verts.push_back(TLN);

	verts.push_back(BLF);
	verts.push_back(BRF);

	verts.push_back(BLF);
	verts.push_back(TLF);

	verts.push_back(BRN);
	verts.push_back(BRF);

	verts.push_back(BRN);
	verts.push_back(TRN);

	verts.push_back(BRF);
	verts.push_back(TRF);

	verts.push_back(TRF);
	verts.push_back(TRN);

	verts.push_back(TRF);
	verts.push_back(TLF);

	verts.push_back(TLN);
	verts.push_back(TLF);

	verts.push_back(TLN);
	verts.push_back(TRN);
}

void AddVertsForLineAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& box, const Rgba8& color /*= Rgba8::COLOR_WHITE*/)
{
	Vec2 halfDim = box.GetHalfDimensions();
	Vec3 center = Vec3(box.GetCenter(), 0);

	Vertex_PCU BL = Vertex_PCU(Vec3(center + Vec3(-halfDim.x, -halfDim.y, 0)), color);
	Vertex_PCU BR = Vertex_PCU(Vec3(center + Vec3(halfDim.x, -halfDim.y, 0)), color);
	Vertex_PCU TL = Vertex_PCU(Vec3(center + Vec3(-halfDim.x, halfDim.y, 0)), color);
	Vertex_PCU TR = Vertex_PCU(Vec3(center + Vec3(halfDim.x, halfDim.y, 0)), color);

	verts.push_back(BL);
	verts.push_back(BR);

	verts.push_back(BL);
	verts.push_back(TL);

	verts.push_back(TR);
	verts.push_back(TL);

	verts.push_back(TR);
	verts.push_back(BR);

}

void AddVertsForSkyBox(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color)
{
	Vec3 point1(bounds.m_maxs);
	Vec3 point2(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_maxs.z);
	Vec3 point3(bounds.m_mins.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 point4(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_maxs.z);
	Vec3 point5(bounds.m_maxs.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 point6(bounds.m_mins.x, bounds.m_maxs.y, bounds.m_mins.z);
	Vec3 point7(bounds.m_mins.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec3 point8(bounds.m_maxs.x, bounds.m_mins.y, bounds.m_mins.z);
	Vec2 offset((bounds.m_maxs.x - bounds.m_mins.x) * 0.00001f, (bounds.m_maxs.y - bounds.m_mins.y) * 0.00001f);
	AddVertsForQuad3D(verts, point2, point1, point3, point4, color, AABB2(Vec2(0.25f, 2.f / 3.f) + offset, Vec2(0.5f, 1.f) - offset));
	AddVertsForQuad3D(verts, point7, point8, point6, point5, color, AABB2(Vec2(0.25f, 0.f) + offset, Vec2(0.5f, 1.f / 3.f) - offset));//-z
	AddVertsForQuad3D(verts, point6, point5, point2, point1, color, AABB2(Vec2(0.25f, 1.f / 3.f) + offset, Vec2(0.5f, 2.f / 3.f) - offset));//y
	AddVertsForQuad3D(verts, point8, point7, point4, point3, color, AABB2(Vec2(0.75f, 1.f / 3.f) + offset, Vec2(1.f, 2.f / 3.f) - offset));//-y
	AddVertsForQuad3D(verts, point5, point8, point1, point4, color, AABB2(Vec2(0.5f, 1.f / 3.f) + offset, Vec2(0.75f, 2.f / 3.f) - offset));//x
	AddVertsForQuad3D(verts, point7, point6, point3, point2, color, AABB2(Vec2(0.f, 1.f / 3.f) + offset, Vec2(0.25f, 2.f / 3.f) - offset));//-x
}

void AddVertForZHexagon(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 center, float circumradius, const Rgba8& color /*= Rgba8::COLOR_WHITE*/)
{
	const int NUM_SIDES = 6;
	const float DEGREES_PER_SIDE = 360.0f / NUM_SIDES;
	const unsigned int startIndex = static_cast<unsigned int>(verts.size());

	// Add center vertex
	Vertex_PCU centerVert;
	centerVert.m_position = center;
	centerVert.m_color = color;
	centerVert.m_uvTexCoords = Vec2(0.0f, 0.0f);  // UVs not used but initialized
	verts.push_back(centerVert);

	float outerR = (2 / sqrtf(3)) * circumradius;

	// Add vertices for the hexagon points
	for (int i = 0; i < NUM_SIDES; i++)
	{
		float angleDegrees = i * DEGREES_PER_SIDE - DEGREES_PER_SIDE / 2;
		float sinAngle = SinDegrees(angleDegrees);
		float cosAngle = CosDegrees(angleDegrees);

		Vertex_PCU vert;
		vert.m_position.x = center.x + (outerR * sinAngle);
		vert.m_position.y = center.y + (outerR * cosAngle);
		vert.m_position.z = center.z;
		vert.m_color = color;
		vert.m_uvTexCoords = Vec2(0.0f, 0.0f);  // UVs not used but initialized

		verts.push_back(vert);
	}

	// Add indices to form triangles
	for (int i = 0; i < NUM_SIDES; i++)
	{
		indexes.push_back(startIndex);  // Center vertex
		indexes.push_back(startIndex + 1 + ((i + 1) % NUM_SIDES));  // Next vertexs
		indexes.push_back(startIndex + 1 + i);  // Current vertex
	}
}

void AddVertForZHexagonOutline(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 center, float circumradius, const Rgba8& color, float lineThickness)
{
	const int NUM_SIDES = 6;
	const float DEGREES_PER_SIDE = 360.0f / NUM_SIDES;
	float radius = lineThickness * 0.5f;

	Vec2 points[NUM_SIDES];

	float outerR = (2 / sqrtf(3)) * circumradius;
	for (int i = 0; i < NUM_SIDES; i++)
	{
		float angleDegrees = i * DEGREES_PER_SIDE;
		float sinAngle = SinDegrees(angleDegrees);
		float cosAngle = CosDegrees(angleDegrees);

		float x = center.x + outerR * cosAngle;
		float y = center.y + outerR * sinAngle;

		points[i] = Vec2(x, y);

		//AddVertsForSphere(verts, indexes, Vec3(x, y, 1.f), 0.015f);
	}

	for (int i = 0; i < NUM_SIDES; i++)
	{
		Vec2 start = points[i];
		Vec2 end = points[(i + 1) % NUM_SIDES];

		Vec2 forwardStart = radius * (start - Vec2(center.x, center.y)).GetNormalized();
		Vec2 forwardEnd = radius * (end - Vec2(center.x, center.y)).GetNormalized();

		Vec3 endTop = Vec3(end + forwardEnd, center.z);
		Vec3 endBottom = Vec3(end - forwardEnd, center.z);
		Vec3 startTop = Vec3(start + forwardStart, center.z);
		Vec3 startBottom = Vec3(start - forwardStart, center.z);

		AddVertsForQuad3D(verts, indexes, endBottom, startBottom, endTop, startTop, color);
	}
}

//void CalculateTangentSpaceBasisVectors(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, bool computeNormals /*= true*/, bool computeTangents /*= true*/)
//{
//	if (!computeNormals && !computeTangents)
//	{
//		return;
//	}
//
//	for (auto& vertex : vertexes)
//	{
//		if (computeNormals) {
//			vertex.m_normal = Vec3();
//		}
//		if (computeTangents) {
//			vertex.m_tangent = Vec3();
//			vertex.m_bitangent = Vec3();
//		}
//	}
//
//	// Process each triangle
//	for (size_t i = 0; i < indexes.size(); i += 3)
//	{
//		Vertex_PCUTBN& v0 = vertexes[indexes[i]];
//		Vertex_PCUTBN& v1 = vertexes[indexes[i + 1]];
//		Vertex_PCUTBN& v2 = vertexes[indexes[i + 2]];
//
//		// Calculate triangle edges and UV differences
//		Vec3 edge1 = v1.m_position - v0.m_position;
//		Vec3 edge2 = v2.m_position - v0.m_position;
//		Vec2 deltaUV1 = v1.m_uvTexCoords - v0.m_uvTexCoords;
//		Vec2 deltaUV2 = v2.m_uvTexCoords - v0.m_uvTexCoords;
//
//		// Calculate normal
//		if (computeNormals)
//		{
//			Vec3 normal = CrossProduct3D(edge1, edge2);
//
//			// Add to vertex normals
//			v0.m_normal += normal;
//			v1.m_normal += normal;
//			v2.m_normal += normal;
//
//			// Normalize
//			v0.m_normal.Normalize();
//			v1.m_normal.Normalize();
//			v2.m_normal.Normalize();
//		}
//
//		if (computeTangents)
//		{
//			float deltaU0 = v1.m_uvTexCoords.x - v0.m_uvTexCoords.x;
//			float deltaU1 = v2.m_uvTexCoords.x - v0.m_uvTexCoords.x;
//
//			float deltaV0 = v1.m_uvTexCoords.y - v0.m_uvTexCoords.y;
//			float deltaV1 = v2.m_uvTexCoords.y - v0.m_uvTexCoords.y;
//
//			float r = 1.f / ((deltaU0 * deltaV1) - (deltaU1 * deltaV0));
//
//			Vec3 tangent = r * ((deltaV1 * edge1) - (deltaV0 * edge2));
//			Vec3 bitangent = r * ((deltaU0 * edge2) - (deltaU1 * edge1));
//
//			vertexes[indexes[i]].m_tangent += tangent.GetNormalized();
//			vertexes[indexes[i + 1]].m_tangent += tangent.GetNormalized();
//			vertexes[indexes[i + 2]].m_tangent += tangent.GetNormalized();
//
//			vertexes[indexes[i]].m_bitangent += bitangent.GetNormalized();
//			vertexes[indexes[i + 1]].m_bitangent += bitangent.GetNormalized();
//			vertexes[indexes[i + 2]].m_bitangent += bitangent.GetNormalized();
//		}
//	}
//
//	// Normalize and orthogonalize
//	for (auto& vertex : vertexes)
//	{
//		if (computeNormals)
//		{
//			vertex.m_normal.Normalize();
//		}
//
//		if (computeTangents)
//		{
//			vertex.m_normal.Normalize();
//			vertex.m_tangent.Normalize();
//			vertex.m_bitangent.Normalize();
//
//			float dP = DotProduct3D(vertex.m_normal, vertex.m_tangent);
//			vertex.m_tangent = vertex.m_tangent - vertex.m_normal * dP;
//			vertex.m_tangent.Normalize();
//
//			if (DotProduct3D(CrossProduct3D(vertex.m_normal, vertex.m_tangent), vertex.m_bitangent) < 0.f)
//			{
//				vertex.m_tangent *= -1.f;
//			}
//		}
//	}
//}

void CalculateTangentSpaceBasisVectors(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, bool computeNormals /*= true*/, bool computeTangents /*= true*/)
{
	for (size_t i = 0; i < indexes.size(); i += 3)
	{
		unsigned int i0 = indexes[i];
		unsigned int i1 = indexes[i + 1];
		unsigned int i2 = indexes[i + 2];

		// Vertex pos
		Vec3& p0 = vertexes[i0].m_position;
		Vec3& p1 = vertexes[i1].m_position;
		Vec3& p2 = vertexes[i2].m_position;

		// Texture coordinates
		Vec2& uv0 = vertexes[i0].m_uvTexCoords;
		Vec2& uv1 = vertexes[i1].m_uvTexCoords;
		Vec2& uv2 = vertexes[i2].m_uvTexCoords;

		// Edge calculation
		Vec3 e0 = p1 - p0;
		Vec3 e1 = p2 - p0;

		if (computeNormals)
		{
			Vec3 N = CrossProduct3D(e0, e1);
			N.Normalize();

			vertexes[i0].m_normal += N;
			vertexes[i1].m_normal += N;
			vertexes[i2].m_normal += N;
		}

		if (computeTangents)
		{
			float deltaU0 = uv1.x - uv0.x;
			float deltaU1 = uv2.x - uv0.x;

			float deltaV0 = uv1.y - uv0.y;
			float deltaV1 = uv2.y - uv0.y;

			float r = 1.f / ((deltaU0 * deltaV1) - (deltaU1 * deltaV0));

			Vec3 tangent = r * ((deltaV1 * e0) - (deltaV0 * e1));
			Vec3 bitangent = r * ((deltaU0 * e1) - (deltaU1 * e0));

			vertexes[i0].m_tangent += tangent.GetNormalized();
			vertexes[i1].m_tangent += tangent.GetNormalized();
			vertexes[i2].m_tangent += tangent.GetNormalized();

			vertexes[i0].m_bitangent += bitangent.GetNormalized();
			vertexes[i1].m_bitangent += bitangent.GetNormalized();
			vertexes[i2].m_bitangent += bitangent.GetNormalized();
		}
	}

	for (size_t i = 0; i < vertexes.size(); i++)
	{
		vertexes[i].m_normal.Normalize();
		vertexes[i].m_tangent.Normalize();
		vertexes[i].m_bitangent.Normalize();

		// Gram-Schmidt orthonormalization
		float dP = DotProduct3D(vertexes[i].m_normal, vertexes[i].m_tangent);
		vertexes[i].m_tangent = vertexes[i].m_tangent - vertexes[i].m_normal * dP;
		vertexes[i].m_tangent.Normalize();

		if (DotProduct3D(CrossProduct3D(vertexes[i].m_normal, vertexes[i].m_tangent), vertexes[i].m_bitangent) < 0.f)
		{
			vertexes[i].m_tangent *= -1.f;
		}
	}
}