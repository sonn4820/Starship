#include "Game/GameCommon.hpp"
#include "GameExtern.hpp"

void DrawDebugRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color)
{
	constexpr int RING_NUM_SIDES = 32;
	constexpr int RING_NUM_TRIS = 2 * RING_NUM_SIDES;
	constexpr int RING_NUM_VERTS = 3 * RING_NUM_TRIS;
	constexpr float DEGREES_PER_SIDE = 360.f / (float)RING_NUM_SIDES;
	float innerRadius = radius - 0.5f * thickness;
	float outerRadius = radius + 0.5f * thickness;
	Vertex_PCU verts[RING_NUM_VERTS];

	for (int i = 0; i < RING_NUM_SIDES; i++)
	{
		//Angle
		float startDegrees = DEGREES_PER_SIDE * (float)i;
		float endDegrees = DEGREES_PER_SIDE * (float)(i + 1);
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

		int vertIndexA = (6 * i) + 0;
		int vertIndexB = (6 * i) + 1;
		int vertIndexC = (6 * i) + 2;
		int vertIndexD = (6 * i) + 3;
		int vertIndexE = (6 * i) + 4;
		int vertIndexF = (6 * i) + 5;

		verts[vertIndexA].m_position = innerEndPos;
		verts[vertIndexB].m_position = innerStartPos;
		verts[vertIndexC].m_position = outerStartPos;
		verts[vertIndexA].m_color = color;
		verts[vertIndexB].m_color = color;
		verts[vertIndexC].m_color = color;

		verts[vertIndexD].m_position = innerEndPos;
		verts[vertIndexE].m_position = outerStartPos;
		verts[vertIndexF].m_position = outerEndPos;
		verts[vertIndexD].m_color = color;
		verts[vertIndexE].m_color = color;
		verts[vertIndexF].m_color = color;

		g_theRenderer->DrawVertexArray(RING_NUM_VERTS, verts);
	}
}

void DrawDebugLine(Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color)
{
	constexpr int LINE_NUM_VERTS = 6;
	Vertex_PCU verts[LINE_NUM_VERTS];
	float radius = thickness * 0.5f;

	Vec2 vecForward = radius * (endPos - startPos).GetNormalized();
	Vec2 vecLeft = vecForward.GetRotated90Degrees();
	Vec2 vecRight = vecForward.GetRotatedMinus90Degrees();

	Vec3 FowardLeftPos = Vec3(endPos.x + vecForward.x + vecLeft.x, endPos.y + vecForward.y + vecLeft.y, 0.f);
	Vec3 FowardRightPos = Vec3(endPos.x + vecForward.x + vecRight.x, endPos.y + vecForward.y + vecRight.y, 0.f);
	Vec3 BackLeftPos = Vec3(startPos.x - vecForward.x + vecLeft.x, startPos.y - vecForward.y + vecLeft.y, 0.f);
	Vec3 BackRightPos = Vec3(startPos.x - vecForward.x + vecRight.x, startPos.y - vecForward.y + vecRight.y, 0.f);

	verts[0].m_position = FowardLeftPos;
	verts[1].m_position = FowardRightPos;
	verts[2].m_position = BackLeftPos;
	verts[3].m_position = FowardRightPos;
	verts[4].m_position = BackLeftPos;
	verts[5].m_position = BackRightPos;

	for (int i = 0; i < LINE_NUM_VERTS; i++)
	{
		verts[i].m_color = color;
	}

	g_theRenderer->DrawVertexArray(LINE_NUM_VERTS, verts);
}

void DrawDebugGlow(Vec2 const& center, float radius, Rgba8 const& colorInside, Rgba8 const& colorOutside)
{
	constexpr int CIRCLE_NUM_SIDES = 16;
	constexpr int CIRCLE_NUM_TRIS = CIRCLE_NUM_SIDES;
	constexpr int CIRCLE_NUM_VERTS = 3 * CIRCLE_NUM_TRIS;
	constexpr float DEGREES_PER_SIDE = 360.f / (float)CIRCLE_NUM_SIDES;

	Vertex_PCU circleVerts[CIRCLE_NUM_VERTS];
	Vec2 circleVertPos[CIRCLE_NUM_SIDES] = {};

	for (int i = 0; i < CIRCLE_NUM_SIDES; i++)
	{
		float degrees = DEGREES_PER_SIDE * (float)i;
		circleVertPos[i].x = center.x + radius * CosDegrees(degrees);
		circleVertPos[i].y = center.y + radius * SinDegrees(degrees);
	}
	for (int i = 0; i < CIRCLE_NUM_TRIS; i++)
	{
		int startRadiusIndex = i;
		int endRadiusIndex = (i + 1) % CIRCLE_NUM_SIDES;
		int firstVertIndex = (i * 3);
		int secondVertIndex = (i * 3) + 1;
		int thirdVertIndex = (i * 3) + 2;

		Vec2 secondVertOfs = circleVertPos[startRadiusIndex];
		Vec2 thirdVertOfs = circleVertPos[endRadiusIndex];

		circleVerts[firstVertIndex].m_position = Vec3(center.x, center.y, 0.f);
		circleVerts[secondVertIndex].m_position = Vec3(secondVertOfs.x, secondVertOfs.y, 0.f);
		circleVerts[thirdVertIndex].m_position = Vec3(thirdVertOfs.x, thirdVertOfs.y, 0.f);

		circleVerts[firstVertIndex].m_color = colorInside;
		circleVerts[secondVertIndex].m_color = colorOutside;
		circleVerts[thirdVertIndex].m_color = colorOutside;
	}

	g_theRenderer->DrawVertexArray(CIRCLE_NUM_VERTS, circleVerts);
}
