#include "VertexBuffer.h"
#include "ErrorCheck.h"

VertexBuffer::VertexBuffer()
{
	GLCall(glGenBuffers(1, &m_RendererID));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::addBuffer(int size, float* arrayPtr) const
{
	bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), arrayPtr, GL_STATIC_DRAW));
}
