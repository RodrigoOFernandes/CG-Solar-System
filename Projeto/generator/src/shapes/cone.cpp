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

    float sliceStep = (2.0f * M_PI) / slices;
    float stackStep = (float)height / stacks;
    
    for (int i = 0; i < slices; i++) {
        float theta1 = i * sliceStep;
        float theta2 = (i + 1) % slices * sliceStep;  

        float x1 = radius * sin(theta1);
        float z1 = radius * cos(theta1);
        float x2 = radius * sin(theta2);
        float z2 = radius * cos(theta2);
        float y = 0.0f;

        outFile << "0.0 0.0 0.0\n";  
        outFile << x2 << " " << y << " " << z2 << std::endl;
        outFile << x1 << " " << y << " " << z1 << std::endl;
    }

    for (int i = 0; i < slices; i++) {
        float theta1 = i * sliceStep;
        float theta2 = (i + 1) % slices * sliceStep; 

        for (int j = 0; j < stacks; j++) {
            float y1 = j * stackStep;
            float y2 = (j + 1) * stackStep;

            float r1 = radius * (1.0f - (float)j / stacks);     
            float r2 = radius * (1.0f - (float)(j + 1) / stacks); 

            float x1_1 = r1 * sin(theta1);
            float z1_1 = r1 * cos(theta1);
            float x1_2 = r1 * sin(theta2);
            float z1_2 = r1 * cos(theta2);

            float x2_1 = r2 * sin(theta1);
            float z2_1 = r2 * cos(theta1);
            float x2_2 = r2 * sin(theta2);
            float z2_2 = r2 * cos(theta2);

            outFile << x2_1 << " " << y2 << " " << z2_1 << "\n";
            outFile << x1_1 << " " << y1 << " " << z1_1 << "\n";
            outFile << x1_2 << " " << y1 << " " << z1_2 << "\n";
            
            outFile << x2_2 << " " << y2 << " " << z2_2 << "\n";
            outFile << x2_1 << " " << y2 << " " << z2_1 << "\n";
            outFile << x1_2 << " " << y1 << " " << z1_2 << "\n";

        }
    }

    outFile.close();
}
