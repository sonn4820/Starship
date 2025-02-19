#pragma once
#include "Engine/Core/ObjLoader.hpp"

class CPUMesh
{
public:
	CPUMesh();
	CPUMesh(std::vector<Vertex_PCUTBN> vertexes, std::vector<unsigned int> indexes);
	CPUMesh(const std::string& objFileName, const Mat44& transform);
	virtual ~CPUMesh();

	void Load(const std::string& objFileName, const Mat44& transform);
	void AddTint(Rgba8 color);

	std::vector<unsigned int> m_indexes;
	std::vector<Vertex_PCUTBN> m_vertexes;
};

