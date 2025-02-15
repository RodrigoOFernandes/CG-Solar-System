#include "../shapes/cone.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

void generateCone(int radius, int height, int slices, int stacks, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);

    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }

    outFile << "cone" << "\n";

    float sliceStep = (float) 2 * M_PI / slices; 
    float stackHeight = (float) height / stacks; 
    float radiusStep = (float) radius / stacks;  

    for (float a = 0; a < (2 * M_PI); a += sliceStep) {
        float x1 = 0; 
        float y1 = 0;
        float z1 = 0;

        float x3 = (float)radius * std::sin(a); 
        float y3 = 0;
        float z3 = (float)radius * std::cos(a);

        float x2 = (float)radius * std::sin(a + sliceStep); 
        float y2 = 0;
        float z2 = (float)radius * std::cos(a + sliceStep);

        outFile << x3 << " " << y3 << " " << z3 << std::endl;
        outFile << x1 << " " << y1 << " " << z1 << std::endl;
        outFile << x2 << " " << y2 << " " << z2 << std::endl;
    }

    for (int i = 0; i < stacks; i++) {
        float r1 = radius - i * radiusStep; 
        float r2 = radius - (i + 1) * radiusStep; 
        float h1 = i * stackHeight; 
        float h2 = (i + 1) * stackHeight; 

        for (int j = 0; j < slices; j++) {
            float angle1 = j * sliceStep;
            float angle2 = (j + 1) * sliceStep; 

            float x1 = r1 * std::sin(angle1);
            float z1 = r1 * std::cos(angle1);
            float x2 = r1 * std::sin(angle2);
            float z2 = r1 * std::cos(angle2);
            float x3 = r2 * std::sin(angle1);
            float z3 = r2 * std::cos(angle1);
            float x4 = r2 * std::sin(angle2);
            float z4 = r2 * std::cos(angle2);

            float y1 = h1;
            float y2 = h1;
            float y3 = h2;
            float y4 = h2;

            outFile << x1 << " " << y1 << " " << z1 << std::endl;
            outFile << x3 << " " << y3 << " " << z3 << std::endl;
            outFile << x2 << " " << y2 << " " << z2 << std::endl;

            outFile << x2 << " " << y2 << " " << z2 << std::endl;
            outFile << x3 << " " << y3 << " " << z3 << std::endl;
            outFile << x4 << " " << y4 << " " << z4 << std::endl;
        }
    }
    outFile.close();
}