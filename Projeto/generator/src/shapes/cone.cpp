#include "../shapes/cone.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>


void generateCone(int radius, int height, int slices, 
                    int stacks, char* outputFile){

    std::ofstream file(outputFile);

    if(!file.is_open())
    {
        std::cerr << "Error opening outputfile" << outputFile << std::endl;
        return;
    }
    std::vector<float> vertices;

    float sectorStep = 2 * M_PI / slices;
    float stackHeight = height/stacks;
    float radiusStep = radius/stacks;
    float sectorAngle, x, y, z;

    //topo
    vertices.push_back(0.0f);
    vertices.push_back(height / 2.0f);
    vertices.push_back(0.0f);
    //base
    for(int i = 0; i < slices; i++)
    {
        sectorAngle = i * sectorStep;
        x = radius * cos(sectorAngle);
        y = 0;
        z = radius * sin(sectorAngle);

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    //lados
    for(int i = 0; i <= stacks;i++)
    {
        float currentRadius = radius - i * radiusStep;
        float currentHeight = height / 2.0f - i * stackHeight;
        for(int j = 0; j < slices; j++){
            sectorAngle = i * sectorStep;
            x = currentRadius * cos(sectorAngle);
            y = currentHeight;
            z = currentRadius * sin(sectorAngle);
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for (size_t i = 0; i < vertices.size(); i += 3) {
        file << vertices[i] << " " << vertices[i + 1] << " " << vertices[i + 2] << "\n";
    }

}