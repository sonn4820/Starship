#pragma once
#include <string>
#include <vector>
struct Vertex_PCU;
struct Rgba8;
struct Vec2;


//------------------------------------------------------------------------------------------------
void AddVertsForTextTriangles2D(std::vector<Vertex_PCU>& verts, std::string const& text, Vec2 const& startMins, float cellHeight, const Rgba8& color, float cellAspect = 0.56f, bool isFlipped = false, float spacingFraction = 0.2f, int maxCharToDraw = 9999);
float GetSimpleTriangleStringWidth(const std::string& text, float cellHeight, float cellAspect = 0.56f, float spacingFraction = 0.2f);

