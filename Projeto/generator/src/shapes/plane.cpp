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

            outFile << x1 << " " << 0.0f << " " << z1 << "\n"; 
            outFile << x1 << " " << 0.0f << " " << z2 << "\n";
            outFile << x2 << " " << 0.0f << " " << z2 << "\n"; 

            outFile << x1 << " " << 0.0f << " " << z1 << "\n"; 
            outFile << x2 << " " << 0.0f << " " << z2 << "\n"; 
            outFile << x2 << " " << 0.0f << " " << z1 << "\n";
        }
    }
    
    outFile.close();
}
