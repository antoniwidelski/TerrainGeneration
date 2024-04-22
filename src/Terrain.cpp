#include "Terrain.h"

#include <GL/glew.h>
#include "ErrorCheck.h"

#include "stb_image.h"
#include "CommonValues.h"

#include "Perlin.h"

#include <iostream>


void Terrain::generateFromFile(std::string filePath)
{
    int mapX, mapY, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &mapX, &mapY, &channels, 0);

    printf("Height: %d, Width: %d, Channels: %d\n", mapX, mapY, channels);

    int size = mapX;

    //printf("scale: %f\n", scale);

    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            unsigned char* texel = data + (x + size * z);
            unsigned char y = texel[0];
            float yy = (float)y;

            //printf("y: %f\n", yy);
            m_heightVector.push_back(yy);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    draw(size);
}

void Terrain::generatePlane(int size)
{
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            m_heightVector.push_back(0.0f);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    draw(size);
}

void Terrain::generateWeird(int size)
{
    bool i = true;
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            switch (i)
            {
                case true:
                    m_heightVector.push_back(0.0f);
                    break;
                case false:
                    m_heightVector.push_back(3.0f);
                    break;
            }
            m_heightVector.push_back(0.0f);
            i = !i;
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    draw(size);
}

void Terrain::generateRandom(int size)
{
    m_heightVector = getRandom(size);
    draw(size);
}

std::vector<float> Terrain::smoothenBorders(std::vector<float> grid, int size)
{
    for (int i = 0; i < size * size; i++)
    {
        if (i < size || i > size * size - size || i % size == 0 || i % size == size - 1)
        {
            grid[i] = 0.0f;
            //std::cout << i << std::endl;
        }
    }
    return grid;
}

void Terrain::generatePerlinTerrain(int size)
{
    m_gridSize = size;
    if(m_noiseSeed.size()==0)
        m_noiseSeed = getRandom(m_gridSize);
    //m_noiseSeed = smoothenBorders(m_noiseSeed, size);
    m_heightVector = getPlane(m_gridSize);
    std::vector<float> perlinNoise;

    float m_Min = m_noiseSeed[0];

    for (int i = 1; i < m_noiseSeed.size(); i++)
    {
        if (m_noiseSeed[i] < m_Min)
        {
            //std::cout << m_Min << std::endl;
        }
    }

    for (int x = 0; x < m_gridSize; x++)
    {
        for (int y = 0; y < m_gridSize; y++)
        {
            float noise = 0.0f;
            float scale = 1.0f;
            float scaleAcc = 0.0f;

            for (int o = 0; o < m_octave; o++)
            {   
                /*if (m_Min == 0 && o == 1)
                    m_Min = m_noiseSeed[x * m_gridSize + y];
                else if (m_noiseSeed[x * m_gridSize + y] < m_Min && o==1)
                    m_Min = m_noiseSeed[x * m_gridSize + y];*/

                int pitch = m_gridSize / pow(2,o);
                int sampleX1 = (x / pitch) * pitch;
                int sampleY1 = (y / pitch) * pitch;

                int sampleX2 =(sampleX1 + pitch) % m_gridSize;
                int sampleY2 = (sampleY1 + pitch) % m_gridSize;

                float blendX = (float)(x - sampleX1) / (float)pitch;
                float blendY = (float)(y - sampleY1) / (float)pitch;

                blendX = blendX * blendX * (3 - 2 * blendX);
                blendY = blendY * blendY * (3 - 2 * blendY);

                float sampleT = (1.0f - blendX) * m_noiseSeed[sampleY1 * m_gridSize + sampleX1] + blendX * m_noiseSeed[sampleY1 * m_gridSize + sampleX2];
                float sampleB = (1.0f - blendX) * m_noiseSeed[sampleY2 * m_gridSize + sampleX1] + blendX * m_noiseSeed[sampleY2 * m_gridSize + sampleX2];
            
                noise += (blendY * (sampleB - sampleT) + sampleT) * scale;
                scaleAcc += scale;
                scale /= 2;
            }
            //std::cout << scaleAcc << std::endl;
            perlinNoise.push_back((noise -m_Min)/scaleAcc);
            
        }
    }
    
    m_heightVector = perlinNoise;
    scale();

    draw(size);
}

void Terrain::generateSmoothNoise(int size)
{
    m_gridSize = size;
    m_noiseSeed = getRandom(m_gridSize);
    //m_noiseSeed = smoothenBorders(m_noiseSeed, size);
    m_heightVector = m_noiseSeed;
    scale();

    draw(size);
}

std::vector<float> Terrain::normalizeHeight(std::vector<float> grid)
{
    float min = grid[0];
    for (int i = 1; i < grid.size(); i++)
    {
        if (grid[i] < min)
            min = grid[i];
    }
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i] -= min;
    }
    std::cout << min << std::endl;
    return grid;
}

void Terrain::scale()
{
    for (int i = 0; i < m_gridSize * m_gridSize; i++)
    {
        m_heightVector[i] *= m_Scale;
    }
    m_heightVector = normalizeHeight(m_heightVector);
}

void Terrain::draw(int size)
{
    m_vertices.clear();
    m_indices.clear();

    float scale = 2;
    float scaleV = scale * (1.0f / (size - 1));
    //printf("Height Vecotr size: %d", m_heightVector.size());
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            float xx = x * scaleV - (scale * 0.5f);
            float zz = z * scaleV - (scale * 0.5f);

            float yy = m_heightVector[x + (size * z)] / size;
            m_vertices.push_back(xx);
            m_vertices.push_back(yy);
            m_vertices.push_back(zz);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    for (int i = 0; i < (size - 1); i++)
    {
        for (int k = 0; k < (size - 1); k++)
        {
            int offset = i * (size);

            int in0 = k + offset;
            int in1 = k + 1 + offset;
            int in2 = k + size + offset;

            int in3 = k + 1 + offset;
            int in4 = k + size + 1 + offset;
            int in5 = k + size + offset;

            m_indices.push_back(in0);
            m_indices.push_back(in1);
            m_indices.push_back(in2);

            //printf("(%d, %d, %d)\n", in0, in1, in2);

            m_indices.push_back(in3);
            m_indices.push_back(in4);
            m_indices.push_back(in5);

            //printf("(%d, %d, %d)\n", in3, in4, in5);
        }
    }

    

    m_VB.addBuffer(m_vertices.size(), &m_vertices[0]);
    layout = VertexBufferLayout(3, 3);
    m_VA.addArray(m_VB, layout);
    m_IB.addBuffer(m_indices.size(), &m_indices[0]);
}



void Terrain::update(int octave, float scale)
{
    if (octave != m_octave || scale != m_Scale || m_noiseSeed.empty())
    {
        m_octave = octave;
        m_Scale = scale;
        generatePerlinTerrain(128);
        //generateSmoothNoise(256);
    }
}

void Terrain::draw()
{
    m_VA.bind();
    m_IB.bind();
    GLCall(glDrawElements(GL_TRIANGLES, m_indices.size()/3 * sizeof(unsigned int), GL_UNSIGNED_INT, 0));
}

void Terrain::printID()
{
    //printf("VAO: %d, VBO: %d, IBO: %d\n", vao, vbo, ibo);
}
