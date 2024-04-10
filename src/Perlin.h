#include <vector>
#include <cmath>
#include <random>

const int X = 0;
const int Y = 1;
const int Z = 2;

const int PERLIN_SIZE = 256;
const float PERLIN_PERSISTENCE = 0.5f;
const int PERLIN_OCTAVES = 6;

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float grad(int hash, float x, float y, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : y;
    float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float noise(float x, float y, float z, const int* p) {
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);
    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;
    float res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
        grad(p[BA], x - 1, y, z)),
        lerp(u, grad(p[AB], x, y - 1, z),
            grad(p[BB], x - 1, y - 1, z))),
        lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
            grad(p[BA + 1], x - 1, y, z - 1)),
            lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                grad(p[BB + 1], x - 1, y - 1, z - 1))));
    return res * 2 - 1;
}

std::vector<float> generatePerlinNoiseMap(int size) {
    std::vector<float> g(size * size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, PERLIN_SIZE - 1);
    std::vector<int> p(PERLIN_SIZE * PERLIN_SIZE);
    for (int i = 0; i < PERLIN_SIZE * PERLIN_SIZE; i++) {
        p[i] = dis(gen);
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            float x = static_cast<float>(i) / size;
            float y = static_cast<float>(j) / size;
            float noiseValue = 0.0f;
            float amplitude = 1.0f;
            float frequency = 1.0f;
            for (int o = 0; o < PERLIN_OCTAVES; o++) {
                noiseValue += amplitude * noise(x * frequency, y * frequency, 0.0f, p.data());
                amplitude *= PERLIN_PERSISTENCE;
                frequency *= 2.0f;
            }
            g[i * size + j] = noiseValue;
        }
    }
    return g;
}