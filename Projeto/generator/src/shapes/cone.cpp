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

    float sliceStep = (float) 2.0f * M_PI / slices;
    
    //base
    for(int i = 0; i < slices; i++){
        float theta1 = (float) i * sliceStep; 
        float theta2 = (float) (i+1) * sliceStep; 
        float y = 0.0f;
        float z1 = (float) radius * cos(theta1);
        float x1 = (float) radius * sin(theta1);
        float z2 = (float) radius * cos(theta2);
        float x2 = (float) radius * sin(theta2);
        
        outFile << 0.0f << " " << y << " " << 0.0f << std::endl;
        outFile << x1 << " " << y << " " << z1 << std::endl;
        outFile << x2 << " " << y << " " << z2 << std::endl;
    }

    //lados

    for(int i = 0; i < stacks; i++)
    {
        
    }
    outFile.close();
}