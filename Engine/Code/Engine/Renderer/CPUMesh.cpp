#include "CPUMesh.hpp"
#include "Engine/Core/ObjLoader.hpp"
#include "Engine/Core/VertexUtils.hpp"

CPUMesh::CPUMesh()
{

}

CPUMesh::CPUMesh(const std::string& objFileName, const Mat44& transform)
{
	Load(objFileName, transform);
}

CPUMesh::CPUMesh(std::vector<Vertex_PCUTBN> vertexes, std::vector<unsigned int> indexes)
	:m_vertexes(vertexes), m_indexes(indexes)
{

}

CPUMesh::~CPUMesh()
{

}

void CPUMesh::Load(const std::string& objFileName, const Mat44& transform)
{
	bool hasNormals;
	bool hasUVs;
	ObjLoader::Load(objFileName, m_vertexes, m_indexes, hasNormals, hasUVs, transform);

	CalculateTangentSpaceBasisVectors(m_vertexes, m_indexes, !hasNormals, hasUVs);
}

void CPUMesh::AddTint(Rgba8 color)
{
	for (size_t i = 0; i < m_vertexes.size(); i++)
	{
		m_vertexes[i].m_color *= color;
	}
}
