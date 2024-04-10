#include "VertexArray.h"


VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::addArray(VertexBuffer vb, VertexBufferLayout layout)
{
	bind();
	vb.bind();
	GLCall(glVertexAttribPointer(0, layout.getCount(), GL_FLOAT, GL_FALSE, layout.getStride() * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
}
