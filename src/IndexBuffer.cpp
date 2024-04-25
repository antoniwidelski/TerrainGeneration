#include "IndexBuffer.h"
#include "ErrorCheck.h"

IndexBuffer::IndexBuffer()
{
	GLCall(glGenBuffers(1, &m_RendererID));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::addBuffer(int count, unsigned int* arrayPtr) const
{
	bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), arrayPtr, GL_DYNAMIC_DRAW));
}
