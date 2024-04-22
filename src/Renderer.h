#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ErrorCheck.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer() {}
	void draw(VertexArray va, IndexBuffer ib, Shader* shader)
	{
		va.bind();
		ib.bind();
		shader->useShader();
		GLCall(glDrawElements(GL_TRIANGLES, ib.getCount()/3 * sizeof(unsigned int), GL_UNSIGNED_INT, 0));
	}
};