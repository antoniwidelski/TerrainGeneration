#pragma once

class VertexBufferLayout
{
private:
	unsigned int m_Count;
	unsigned int m_Stride;
public:
	VertexBufferLayout() {}
	VertexBufferLayout(unsigned int count, unsigned int stride)
		: m_Count(count), m_Stride(stride) {}

	unsigned int getCount() const { return m_Count; }
	unsigned int getStride() const { return m_Stride; }
};