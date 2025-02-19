#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/OBB2.hpp"
#include "Engine/Math/OBB3.hpp"
#include "Engine/Math/Capsule2.hpp"
#include "Engine/Math/Capsule3.hpp"
#include "Engine/Math/LineSegment2.hpp"
#include "Engine/Math/ConvexShape.hpp"
#include "Engine/Math/Mat44.hpp"
#include <vector>


// Utility
void TransformVertexArrayXY3D(int numVerts, Vertex_PCU* verts, float uniformScaleXY, float rotationDegreesAboutZ, Vec2 const& translationXY);
void TransformVertexArray3D(std::vector<Vertex_PCU>& verts, const Mat44& transform);
AABB2 GetVertexBounds2D(const std::vector<Vertex_PCU>& verts);

// 2D
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Capsule2 const& capsule, Rgba8 const& color);
void AddVertsForCapsule2D(std::vector<Vertex_PCU>& verts, Vec2 const& boneStart, Vec2 const& boneEnd, float radius, Rgba8 const& color);
void AddVertsForDisc2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& color, int sides = 32);
void AddVertsForDisc2DGradient(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, Rgba8 const& innerColor, Rgba8 const& outerColor, int sides = 32);
void AddVertsForRing2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, float radius, float thickness, Rgba8 const& color, int sides = 32);
void AddVertsForAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, Vec2 uvMins = Vec2::ZERO, Vec2 uvMaxs = Vec2::ONE);
void AddVertsForAABB2DOutline(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, OBB2 const& orientedBox, Rgba8 const& color);
void AddVertsForOBB2D(std::vector<Vertex_PCU>& verts, Vec2 const& center, Vec2 const& ibasis, Vec2 const& halfDimension, Rgba8 const& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color);
void AddVertsForLineSegment2D(std::vector<Vertex_PCU>& verts, LineSegment2 const& lineSegment, float thickness, Rgba8 const& color);
void AddVertsForLineSegment2DWith2Colors(std::vector<Vertex_PCU>& verts, Vec2 const& start, Vec2 const& end, float thickness, Rgba8 const& color1, Rgba8 const& color2);
void AddVertsForArrow2D(std::vector<Vertex_PCU>& verts, Vec2 tailPos, Vec2 tipPos, float arrowSize, float lineThickness, Rgba8 const& color);
void AddVertsForWireframeAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);

void AddVertsForConvexPoly2D(std::vector<Vertex_PCU>& verts, ConvexPoly2 const& convex, Rgba8 const& color = Rgba8::COLOR_WHITE);
void AddVertsForConvexPoly2DPlanes(std::vector<Vertex_PCU>& verts, ConvexHull2 const& convex, Rgba8 const& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);
void AddVertsForConvexPoly2DOutline(std::vector<Vertex_PCU>& verts, ConvexPoly2 const& convex, Rgba8 const& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);
void AddVertsForPlane2D(std::vector<Vertex_PCU>& verts, Plane2 const& plane2, Rgba8 const& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);

// 3D
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3DInverseNormal(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForQuad3DNoNormal(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForRoundedQuad3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForAABB3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, AABB3 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForAABB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, AABB3 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForOBB3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, OBB3 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForOBB3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, OBB3 const& bounds, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForSphere(std::vector<Vertex_PCU>& verts, const Vec3& center, float radius, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE, int numLatitudeSlices = 32, int numLongtitudeSlices = 64);
void AddVertsForSphere(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& center, float radius, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE, int numLatitudeSlices = 32, int numLongtitudeSlices = 64);
void AddVertsForSphere(std::vector<Vertex_PCUTBN>& verts, const Vec3& center, float radius, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE, int numLatitudeSlices = 32, int numLongtitudeSlices = 64);
void AddVertsForSphere(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& center, float radius, Rgba8 const& color = Rgba8::COLOR_WHITE, AABB2 const& UVs = AABB2::ZERO_TO_ONE, int numLatitudeSlices = 32, int numLongtitudeSlices = 64);
void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 8);
void AddVertsForCylinder3DNoCap(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 8);
void AddVertsForCylinder3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes , const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCylinder3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes , const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCylinder3DNoCap(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes , const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCapsule3D(std::vector<Vertex_PCU>& verts, Capsule3 capsule, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCapsule3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Capsule3 capsule, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCapsule3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Capsule3 capsule, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForHemisphere3D(std::vector<Vertex_PCU>& verts, Mat44 transform, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForHemisphere3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Mat44 transform, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForHemisphere3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Mat44 transform, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);

void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, const Vec3& base, const Vec3& tip, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCone3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, const Vec3& base, const Vec3& tip, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForCone3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, const Vec3& base, const Vec3& tip, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForArrow3D(std::vector<Vertex_PCU>& verts, const Vec3& start, const Vec3& end, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE, int numSlices = 32);
void AddVertsForZCylinder3D(std::vector<Vertex_PCU>& verts, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, int numSlices = 8, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForZCylinder3D(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, int numSlices = 8, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForZCylinder3D(std::vector<Vertex_PCUTBN>& verts, std::vector<unsigned int>& indexes, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, int numSlices = 8, const Rgba8& color = Rgba8::COLOR_WHITE, const AABB2& UVs = AABB2::ZERO_TO_ONE);
void AddVertsForWireframeQuad3D(std::vector<Vertex_PCU>& verts, const Vec3& bottomLeft, const Vec3& bottomRight, const Vec3& topLeft, const Vec3& topRight, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f, int numSlices = 4);
void AddVertsForWireframeAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& box, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);
void AddVertsForWireframeOBB3D(std::vector<Vertex_PCU>& verts, OBB3 const& box, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);
void AddVertsForWireframeSphere3D(std::vector<Vertex_PCU>& verts, Vec3 const& center, float radius, int numSlices = 16, int numStacks = 8, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);
void AddVertsForWireframeZCylinder3D(std::vector<Vertex_PCU>& verts, Vec2 const& centerXY, FloatRange const& minMaxZ, float radius, float numSlices = 8, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f);
void AddVertsForWireframeHemisphere3D(std::vector<Vertex_PCU>& verts, Mat44 transform, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f, int numSlices = 32);
void AddVertsForWireframeCapsule3D(std::vector<Vertex_PCU>& verts, Capsule3 capsule, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f, int numSlices = 32);
void AddVertsForWireframeCylinder3DNoCap(std::vector<Vertex_PCU>& verts, Vec3 start, Vec3 end, float radius, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.005f, int numSlices = 32);

void AddVertsForLineAABB3D(std::vector<Vertex_PCU>& verts, AABB3 const& box, const Rgba8& color = Rgba8::COLOR_WHITE);
void AddVertsForLineAABB2D(std::vector<Vertex_PCU>& verts, AABB2 const& box, const Rgba8& color = Rgba8::COLOR_WHITE);

void AddVertsForSkyBox(std::vector<Vertex_PCU>& verts, const AABB3& bounds, const Rgba8& color);

void AddVertForZHexagon(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 center, float circumradius, const Rgba8& color = Rgba8::COLOR_WHITE);
void AddVertForZHexagonOutline(std::vector<Vertex_PCU>& verts, std::vector<unsigned int>& indexes, Vec3 center, float circumradius, const Rgba8& color = Rgba8::COLOR_WHITE, float lineThickness = 0.025f);
void CalculateTangentSpaceBasisVectors(std::vector<Vertex_PCUTBN>& vertexes, std::vector<unsigned int>& indexes, bool computeNormals = true, bool computeTangents = true);