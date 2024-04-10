#include "Terrain.h"

#include <GL/glew.h>
#include "ErrorCheck.h"

#include "stb_image.h"
#include "CommonValues.h"

#include "Perlin.h"

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
            heightVector.push_back(yy);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    generate(size);
}

void Terrain::generatePlane(int size)
{
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            heightVector.push_back(0.0f);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    generate(size);
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
                    heightVector.push_back(0.0f);
                    break;
                case false:
                    heightVector.push_back(3.0f);
                    break;
            }
            heightVector.push_back(0.0f);
            i = !i;
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    generate(size);
}

void Terrain::generatePerlinTerrain(int size)
{
    heightVector = generatePerlinNoiseMap(size);
    for (int i = 0; i < heightVector.size(); i++)
    {
        heightVector[i] *= 50;
    }
    generate(size);
}

void Terrain::generate(int size)
{
    float scale = 2;
    float scaleV = scale * (1.0f / (size - 1));
    printf("Height Vecotr size: %d", heightVector.size());
    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            float xx = x * scaleV - (scale * 0.5f);
            float zz = z * scaleV - (scale * 0.5f);

            float yy = heightVector[x + (size * z)] / size;
            vertices.push_back(xx);
            vertices.push_back(yy);
            vertices.push_back(zz);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    triNum = (unsigned int)(2 * (size - 1) * (size - 1));

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

            indices.push_back(in0);
            indices.push_back(in1);
            indices.push_back(in2);

            //printf("(%d, %d, %d)\n", in0, in1, in2);

            indices.push_back(in3);
            indices.push_back(in4);
            indices.push_back(in5);

            //printf("(%d, %d, %d)\n", in3, in4, in5);
        }
    }

    

    m_VB.addBuffer(vertices.size(), &vertices[0]);
    layout = VertexBufferLayout(3, 3);
    m_VA.addArray(m_VB, layout);
    m_IB.addBuffer(indices.size(), &indices[0]);
}

void Terrain::draw()
{
    m_VA.bind();
    m_IB.bind();
    GLCall(glDrawElements(GL_TRIANGLES, indices.size()/3 * sizeof(unsigned int), GL_UNSIGNED_INT, 0));
}

void Terrain::printID()
{
    //printf("VAO: %d, VBO: %d, IBO: %d\n", vao, vbo, ibo);
}
