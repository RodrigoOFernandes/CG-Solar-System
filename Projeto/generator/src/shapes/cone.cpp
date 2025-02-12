#include "../shapes/cone.hpp"



void generateCone(int radius, int height, int slices, 
                    int stacks, char* outputFile){

    std::ofstream file(outputFile);

    if(!file.is_open())
    {
        std::cerr << "Error opening outputfile" << outputFile << std::endl;
        return;
    }

    float sectorStep = 2 * M_PI / slices;
    float sectorAngle, x, y, z;

    //base
    for(int i = 0; i < slices; i++)
    {
        sectorAngle = i * sectorStep;
        x = radius * cos(sectorAngle);
        y = 0;
        z = radius * sin(sectorAngle);

        printf("Vertice: x=%f; y=%f; z=%f;", x, y, z);
    }
}