#pragma once

struct ID3D11Device;
struct ID3D11Buffer;

class VertexBuffer
{
	friend class Renderer;

public:
	VertexBuffer(unsigned int size);
	VertexBuffer(ID3D11Device* device, size_t size, unsigned int stride);
	VertexBuffer(VertexBuffer& copy) = delete;
	virtual ~VertexBuffer();

	void Resize(unsigned int size);

	unsigned int GetSize() const;
	unsigned int GetStride() const;

	void SetIsLinePrimitive(bool value);

private:
	ID3D11Device* m_device = nullptr;
	ID3D11Buffer* m_buffer = nullptr;
	unsigned int m_size = 0;
	unsigned int m_stride = 0;
	bool m_isLinePrimitive = false;
};