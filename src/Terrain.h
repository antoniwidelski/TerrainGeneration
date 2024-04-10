#pragma once

#include <vector>
#include <string>

#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

class Terrain
{
private:
	VertexArray m_VA;
	VertexBuffer m_VB;
	IndexBuffer m_IB;
	VertexBufferLayout layout;

	unsigned int triNum;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	std::vector<float> heightVector;

	void generate(int size);

public:
	Terrain() {}
	void generateFromFile(std::string filepath);
	void generatePlane(int size);
	void generateWeird(int size);
	void generatePerlinTerrain(int size);
	
	void draw();

	void printID();

	VertexArray getVertexArray() { return m_VA; }
	IndexBuffer getIndexBuffer() { return m_IB; }
};

