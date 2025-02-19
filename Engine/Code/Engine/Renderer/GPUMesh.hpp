#pragma once
#include "Engine/Renderer/CPUMesh.hpp"
#include "Engine/Renderer/Renderer.hpp"

class GPUMesh
{
public:
	GPUMesh(Renderer* renderer);
	GPUMesh(Renderer* renderer, const CPUMesh* cpuMesh);
	virtual ~GPUMesh();

	void Create(const CPUMesh* cpuMesh);
	void Render() const;

protected:
	VertexBuffer* m_vertexBuffer = nullptr;
	IndexBuffer* m_indexBuffer = nullptr;
	Renderer* m_renderer = nullptr;
	int m_indexesSize = 0;
};

