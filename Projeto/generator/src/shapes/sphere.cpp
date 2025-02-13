#include "../shapes/sphere.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

void generateSphere(int radius, int slices, int stacks, char* outputFile){
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    if(!outFile.is_open())
    {
        std::cerr << "Error opening outputfile" << outputFile << std::endl;
        return;
    }

    outFile << "sphere" << "\n";

    std::vector<float> vertices;

    float x,y,z, xy;

    float sectorStep = 2 * M_PI / slices;
    float stackStep = M_PI / stacks;
    float sectorAngle, stackAngle;

    for(int i = 0; i <= stacks; i++)
    {
        stackAngle = M_PI / 2 - i * stackStep;        
        xy = radius * cosf(stackAngle);            
        z = radius * sinf(stackAngle);        
        for(int j = 0; j <= slices; j++)
        {
            sectorAngle = j * sectorStep;           

            x = xy * cosf(sectorAngle);           
            y = xy * sinf(sectorAngle);            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (size_t i = 0; i < vertices.size(); i += 3) {
        outFile << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << "\n";
    }
    outFile.close();   
}