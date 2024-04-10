#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer();

	void bind() const;
	void addBuffer(int size, float* arrayPtr) const;

	~VertexBuffer() {}
};