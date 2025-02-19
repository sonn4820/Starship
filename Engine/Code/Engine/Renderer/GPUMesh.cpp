#include "GPUMesh.hpp"

GPUMesh::GPUMesh(Renderer* renderer)
	:m_renderer(renderer)
{

}

GPUMesh::GPUMesh(Renderer* renderer, const CPUMesh* cpuMesh)
	:m_renderer(renderer)
{
	Create(cpuMesh);
}

GPUMesh::~GPUMesh()
{
	delete m_vertexBuffer;
	m_vertexBuffer = nullptr;

	delete m_indexBuffer;
	m_indexBuffer = nullptr;
}

void GPUMesh::Create(const CPUMesh* cpuMesh)
{
	m_indexesSize = (int)cpuMesh->m_indexes.size();
	m_vertexBuffer = m_renderer->CreateVertexBuffer(sizeof(Vertex_PCUTBN) * (unsigned int)cpuMesh->m_vertexes.size());
	m_indexBuffer = m_renderer->CreateIndexBuffer(sizeof(unsigned int) * (unsigned int)cpuMesh->m_indexes.size());
	m_renderer->CopyCPUToGPU(cpuMesh->m_vertexes.data(), (int)(cpuMesh->m_vertexes.size() * sizeof(Vertex_PCUTBN)), m_vertexBuffer);
	m_renderer->CopyCPUToGPU(cpuMesh->m_indexes.data(), (int)(cpuMesh->m_indexes.size() * sizeof(unsigned int)), m_indexBuffer);
}

void GPUMesh::Render() const
{
	m_renderer->DrawIndexedBuffer(m_vertexBuffer, m_indexBuffer, m_indexesSize, 0, VertexType::Vertex_PCUTBN);
}
