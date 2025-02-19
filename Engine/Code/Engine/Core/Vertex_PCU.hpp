#pragma once
#include "Engine/Math/Vec3.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/Rgba8.hpp"

enum class VertexType
{
	Vertex_PCU,
	Vertex_PCUTBN,
	COUNT
};

struct Vertex_PCU
{
public:

	Vertex_PCU() = default;
	Vertex_PCU(float x, float y, float z, 
		unsigned char r, unsigned char g, unsigned char b, unsigned char a,
		float u = 0.f, float v = 0.f);
	explicit Vertex_PCU(Vec3 const& position, Rgba8 const& color = Rgba8::COLOR_WHITE, Vec2 const& uvTexCoords = Vec2(0, 1));

	Vec3 m_position;
	Rgba8 m_color;
	Vec2 m_uvTexCoords;

};

struct Vertex_PCUTBN
{
	Vertex_PCUTBN() = default;
	Vertex_PCUTBN(float px, float py, float pz, 
		unsigned char r, unsigned char g, unsigned char b, unsigned char a,
		float u, float v,
		float tx, float ty, float tz,
		float bx, float by, float bz,
		float nx, float ny, float nz);
	explicit Vertex_PCUTBN(Vec3 const& position, Rgba8 const& color, Vec2 const& uvTexCoords,
		Vec3 const& tangent, Vec3 const& bitangent, Vec3 const& normal);

	Vec3 m_position = Vec3();
	Rgba8 m_color = Rgba8::COLOR_WHITE;
	Vec2 m_uvTexCoords = Vec2();
	Vec3 m_tangent = Vec3();
	Vec3 m_bitangent = Vec3();
	Vec3 m_normal = Vec3();

	//size 60
};