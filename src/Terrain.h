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
	VertexBufferLayout m_layout;

	int m_gridSize;
	int m_octave;
	float m_Scale;

	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;

	std::vector<float> m_noiseSeed;
	std::vector<float> m_heightVector;

	void scale();
	void draw(int size);

public:
	Terrain() { m_Scale = 1; }
	~Terrain() {}

	void generateFromFile(std::string filepath);
	void generatePlane(int size);
	void generateRandom(int size);
	void generatePerlinTerrain(int size);
	
	std::vector<float> normalizeHeight(std::vector<float> grid);

	void clearSeed() { m_noiseSeed.clear(); }

	void update(int gridSize, float scale);
	void draw();

	VertexArray getVertexArray() { return m_VA; }
	IndexBuffer getIndexBuffer() { return m_IB; }
};

