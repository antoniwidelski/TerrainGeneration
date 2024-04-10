#pragma once

#include "ErrorCheck.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	void bind() const;
	void addArray(VertexBuffer vb, VertexBufferLayout layout);
};