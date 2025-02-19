#include "Engine/Renderer/VertexBuffer.hpp"
#include "Engine/Renderer/Renderer.hpp"

VertexBuffer::VertexBuffer(unsigned int size)
{
	m_size = size;
}

VertexBuffer::VertexBuffer(ID3D11Device* device, size_t size, unsigned int stride)
	:m_device(device), m_size((unsigned int)size), m_stride(stride)
{

}

VertexBuffer::~VertexBuffer()
{
	DX_SAFE_RELEASE(m_buffer);
}

void VertexBuffer::Resize(unsigned int size)
{
	m_size = size;
}

unsigned int VertexBuffer::GetSize() const
{
	return m_size;
}

unsigned int VertexBuffer::GetStride() const
{
	return m_stride;
}

void VertexBuffer::SetIsLinePrimitive(bool value)
{
	m_isLinePrimitive = value;
}
