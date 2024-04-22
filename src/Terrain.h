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
	void generateFromFile(std::string filepath);
	void generatePlane(int size);
	void generateWeird(int size);
	void generateRandom(int size);
	void generatePerlinTerrain(int size);
	void generateSmoothNoise(int size);
	
	std::vector<float> normalizeHeight(std::vector<float> grid);

	std::vector<float> mergeLayers(std::vector<float> baseLayer, std::vector<float> topLayer, int octave);
	std::vector<float> smoothenBorders(std::vector<float> grid, int size);

	void clearSeed() { m_noiseSeed.clear(); }
	void clear();

	void update(int gridSize, float scale);
	void draw();

	void printID();

	VertexArray getVertexArray() { return m_VA; }
	IndexBuffer getIndexBuffer() { return m_IB; }
};

