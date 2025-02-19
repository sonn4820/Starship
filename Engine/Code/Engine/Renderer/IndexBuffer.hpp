#pragma once

struct ID3D11Buffer;

class IndexBuffer
{
	friend class Renderer;

public:
	IndexBuffer(size_t size);
	IndexBuffer(IndexBuffer& copy) = delete;
	virtual ~IndexBuffer();

	ID3D11Buffer* m_buffer = nullptr;
	size_t m_size = 0;
};