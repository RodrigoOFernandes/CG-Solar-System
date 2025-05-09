#include "../shapes/torus.hpp"
#include <fstream>
#include <cmath>
#include <string>

struct Vertex {
    float x, y, z;     // posição
    float nx, ny, nz;  // normal
    float u, v;        // coordenadas de textura

    void writeTo(std::ofstream& out) const {
        out << x << " " << y << " " << z << " "
            << nx << " " << ny << " " << nz << " "
            << u << " " << v << "\n";
    }
};

Vertex computeTorusVertex(float minorR, float majorR, float sliceAngle, float stackAngle) {
    float cosSlice = cosf(sliceAngle);
    float sinSlice = sinf(sliceAngle);
    float cosStack = cosf(stackAngle);
    float sinStack = sinf(stackAngle);

    float x = (majorR + minorR * cosSlice) * cosStack;
    float y = (majorR + minorR * cosSlice) * sinStack;
    float z = minorR * sinSlice;

    float nx = cosSlice * cosStack;
    float ny = cosSlice * sinStack;
    float nz = sinSlice;

    float u = stackAngle / (2.0f * M_PI);
    float v = sliceAngle / (2.0f * M_PI);

    return {x, y, z, nx, ny, nz, u, v};
}

void generateTorus(int minorRadius, int majorRadius, int stacks, int slices, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);

    outFile << "torus\n";

    float sliceStep = 2.0f * M_PI / slices;
    float stackStep = 2.0f * M_PI / stacks;

    for (int i = 0; i < slices; ++i) {
        float slice1 = i * sliceStep;
        float slice2 = (i + 1) * sliceStep;

        for (int j = 0; j < stacks; ++j) {
            float stack1 = j * stackStep;
            float stack2 = (j + 1) * stackStep;

            // 4 vértices do quadrado
            Vertex v1 = computeTorusVertex(minorRadius, majorRadius, slice1, stack1);
            Vertex v2 = computeTorusVertex(minorRadius, majorRadius, slice1, stack2);
            Vertex v3 = computeTorusVertex(minorRadius, majorRadius, slice2, stack1);
            Vertex v4 = computeTorusVertex(minorRadius, majorRadius, slice2, stack2);

            // 2 triângulos
            v1.writeTo(outFile);
            v2.writeTo(outFile);
            v3.writeTo(outFile);

            v2.writeTo(outFile);
            v4.writeTo(outFile);
            v3.writeTo(outFile);
        }
    }
}

