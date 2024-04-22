#include <vector>
#include <math.h>
#include <random>

std::vector<float> getRandom(int size)
{
    std::vector<float> randomGrid;

    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            float value = (float)std::rand() / (float)RAND_MAX*10;
            randomGrid.push_back(value);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }
    return randomGrid;
}

std::vector<float> getPlane(int size)
{
    std::vector<float> grid;

    for (int z = 0; z < size; z++)
    {
        for (int x = 0; x < size; x++)
        {
            grid.push_back(0.0f);
            //printf("X: %f, Y: %f, Z: %f\n", xx, yy, zz);
        }
    }

    return grid;
}

std::vector<float> getPerlinLayer(int level, std::vector<float> randomGrid, std::vector<float> grid)
{
    std::vector<float> newLayer;
    newLayer = getPlane(65);
    int power = pow(2, level);
    for (int z = 0; z < power; z++)
    {
        for (int x = 0; x < power; x++)
        {
            newLayer[z * 64 + 64 / power * x] = randomGrid[z * 64 + 64 / power * x];
        }
        newLayer[(z+1)*64] = randomGrid[(z + 1) * 64];
    }
    return grid;
}
