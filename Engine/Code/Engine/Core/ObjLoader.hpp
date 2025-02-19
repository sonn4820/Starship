#pragma once
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Math/Mat44.hpp"
#include <vector>

struct Vertex
{
	int m_vertexPositionIndex = -1;
	int m_vertexTextureCoordinateIndex = -1;
	int m_vertexNormalIndex = -1;
};

struct VertexHash
{
	std::size_t operator()(const Vertex& k) const
	{
		// My Random Hash
		std::size_t h1 = std::hash<int>{}(k.m_vertexPositionIndex);
		std::size_t h2 = std::hash<int>{}(k.m_vertexTextureCoordinateIndex);
		std::size_t h3 = std::hash<int>{}(k.m_vertexNormalIndex);
		return (h1 * 19 + h2) * 31 + h3;
	}
};

struct VertexEqual
{
	bool operator()(const Vertex& lhs, const Vertex& rhs) const
	{
		return lhs.m_vertexPositionIndex == rhs.m_vertexPositionIndex &&
			lhs.m_vertexTextureCoordinateIndex == rhs.m_vertexTextureCoordinateIndex &&
			lhs.m_vertexNormalIndex == rhs.m_vertexNormalIndex;
	}
};
struct Triangle
{
	Triangle(int v0, int v1, int v2,
		int t0 = -1, int t1 = -1, int t2 = -1,
		int n0 = -1, int n1 = -1, int n2 = -1,
		const Rgba8& color = Rgba8::COLOR_WHITE)
		: m_color(color)
	{
		m_vertexPositionIndex[0] = v0;
		m_vertexPositionIndex[1] = v1;
		m_vertexPositionIndex[2] = v2;

		m_vertexTextureCoordinateIndex[0] = t0;
		m_vertexTextureCoordinateIndex[1] = t1;
		m_vertexTextureCoordinateIndex[2] = t2;

		m_vertexNormalIndex[0] = n0;
		m_vertexNormalIndex[1] = n1;
		m_vertexNormalIndex[2] = n2;
	}

	int m_vertexPositionIndex[3]{ -1,-1,-1 };
	Rgba8 m_color;
	int m_vertexTextureCoordinateIndex[3]{ -1,-1,-1 };
	int m_vertexNormalIndex[3]{ -1,-1,-1 };
};

struct Face
{
	std::vector<unsigned int> m_vertPositionIndexes;
	std::vector<unsigned int> m_vertTextureIndexes;
	std::vector<unsigned int> m_vertNormalIndexes;
	Rgba8 m_color;
};

class ObjLoader
{
public:
	static bool Load(const std::string& fileName,
		std::vector<Vertex_PCUTBN>& outVertexes, std::vector<unsigned int>& outIndexes,
		bool& outHasNormals, bool& outHasUVs, const Mat44& transform = Mat44());
};

