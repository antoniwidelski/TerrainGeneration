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
        }
    }

    return grid;
}