#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;

public:
	IndexBuffer();
	void bind() const;
	void addBuffer(int count, unsigned int* arrayPtr);

	unsigned int getCount() const;

	~IndexBuffer() {}
};