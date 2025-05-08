#include "../shapes/cone.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

void normalize(float& x, float& y, float& z) {
    float length = sqrt(x * x + y * y + z * z);
    if (length != 0.0f) {
        x /= length;
        y /= length;
        z /= length;
    }
}

void generateCone(int radius, int height, int slices, int stacks, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);

    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }

    outFile << "cone" << "\n";

    float sliceStep = (2.0f * M_PI) / slices;
    float stackStep = (float)height / stacks;

    // Base
    for (int i = 0; i < slices; i++) {
        float theta1 = i * sliceStep;
        float theta2 = (i + 1) % slices * sliceStep;

        float x1 = radius * sin(theta1);
        float z1 = radius * cos(theta1);
        float x2 = radius * sin(theta2);
        float z2 = radius * cos(theta2);
        float y = 0.0f;

        // Centro
        outFile << "0.0 0.0 0.0  0.0 -1.0 0.0  0.5 0.5\n";
        // Borda 1
        outFile << x2 << " " << y << " " << z2 << "  0.0 -1.0 0.0  "
                << (0.5 + 0.5 * sin(theta2)) << " " << (0.5 + 0.5 * cos(theta2)) << "\n";
        // Borda 2
        outFile << x1 << " " << y << " " << z1 << "  0.0 -1.0 0.0  "
                << (0.5 + 0.5 * sin(theta1)) << " " << (0.5 + 0.5 * cos(theta1)) << "\n";
    }

    // Corpo lateral
    for (int i = 0; i < slices; i++) {
        float theta1 = i * sliceStep;
        float theta2 = (i + 1) % slices * sliceStep;

        for (int j = 0; j < stacks; j++) {
            float y1 = j * stackStep;
            float y2 = (j + 1) * stackStep;

            float r1 = radius * (1.0f - (float)j / stacks);
            float r2 = radius * (1.0f - (float)(j + 1) / stacks);

            float x11 = r1 * sin(theta1);
            float z11 = r1 * cos(theta1);
            float x12 = r1 * sin(theta2);
            float z12 = r1 * cos(theta2);
            float x21 = r2 * sin(theta1);
            float z21 = r2 * cos(theta1);
            float x22 = r2 * sin(theta2);
            float z22 = r2 * cos(theta2);

            // Normais
            float nx11 = x11, ny11 = radius / (float)height, nz11 = z11;
            float nx12 = x12, ny12 = radius / (float)height, nz12 = z12;
            float nx21 = x21, ny21 = radius / (float)height, nz21 = z21;
            float nx22 = x22, ny22 = radius / (float)height, nz22 = z22;

            normalize(nx11, ny11, nz11);
            normalize(nx12, ny12, nz12);
            normalize(nx21, ny21, nz21);
            normalize(nx22, ny22, nz22);

            // Texture coords
            float u1 = (float)i / slices;
            float u2 = (float)(i + 1) / slices;
            float v1 = (float)j / stacks;
            float v2 = (float)(j + 1) / stacks;

            // Triângulo 1
            outFile << x21 << " " << y2 << " " << z21 << "  " << nx21 << " " << ny21 << " " << nz21 << "  " << u1 << " " << v2 << "\n";
            outFile << x11 << " " << y1 << " " << z11 << "  " << nx11 << " " << ny11 << " " << nz11 << "  " << u1 << " " << v1 << "\n";
            outFile << x12 << " " << y1 << " " << z12 << "  " << nx12 << " " << ny12 << " " << nz12 << "  " << u2 << " " << v1 << "\n";

            // Triângulo 2
            outFile << x22 << " " << y2 << " " << z22 << "  " << nx22 << " " << ny22 << " " << nz22 << "  " << u2 << " " << v2 << "\n";
            outFile << x21 << " " << y2 << " " << z21 << "  " << nx21 << " " << ny21 << " " << nz21 << "  " << u1 << " " << v2 << "\n";
            outFile << x12 << " " << y1 << " " << z12 << "  " << nx12 << " " << ny12 << " " << nz12 << "  " << u2 << " " << v1 << "\n";
        }
    }

    outFile.close();
}

