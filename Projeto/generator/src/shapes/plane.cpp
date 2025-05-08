#include "../shapes/plane.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

void generatePlane(int length, int divisions, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    outFile << "plane" << "\n";

    float halfLength = (float) length / 2.0f;
    float step = (float) length / divisions;
    
    for (int i = 0; i < divisions; i++) {
        for (int j = 0; j < divisions; j++) {
            float x1 = -halfLength + i * step;
            float x2 = x1 + step;
            float z1 = -halfLength + j * step;
            float z2 = z1 + step;

            float u1 = static_cast<float>(i) / static_cast<float>(divisions);
            float u2 = static_cast<float>(i + 1) / static_cast<float>(divisions);
            float v1 = static_cast<float>(j) / static_cast<float>(divisions);
            float v2 = static_cast<float>(j + 1) / static_cast<float>(divisions);


            outFile << x1 << " " << 0.0f << " " << z1 << " 0.0 1.0 0.0 " << u1 << " " << v1 << "\n"; // Bottom-left
            outFile << x1 << " " << 0.0f << " " << z2 << " 0.0 1.0 0.0 " << u1 << " " << v2 << "\n"; // Top-left
            outFile << x2 << " " << 0.0f << " " << z2 << " 0.0 1.0 0.0 " << u2 << " " << v2 << "\n"; // Top-right
            
            outFile << x1 << " " << 0.0f << " " << z1 << " 0.0 1.0 0.0 " << u1 << " " << v1 << "\n"; // Bottom-left
            outFile << x2 << " " << 0.0f << " " << z2 << " 0.0 1.0 0.0 " << u2 << " " << v2 << "\n"; // Top-right
            outFile << x2 << " " << 0.0f << " " << z1 << " 0.0 1.0 0.0 " << u2 << " " << v1 << "\n"; // Bottom-right
            
        }
    }
    
    outFile.close();
}
