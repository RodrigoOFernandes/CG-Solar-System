#include "include/primitives/sphere.h"


void generateSphere(int radius, int slices, int stacks, char* outputFile){
    std::ofstream file(outputFile);

    if(!file.is_open())
    {
        std::cerr << "Error opening outputfile" << outputFile << std::endl;
        return;
    }

    file << (slices + 1) * (stacks + 1) << std::endl;

    for(int i = 0; i <= stacks; i++)
    {
        float phi =  M_PI * i / stacks;
        for(int j = 0; j <= slices; j++)
        {
            float theta =  2 * M_PI * j / slices;
            float x = radius * sin(phi) * cos(theta);
            float y = radius * cos(phi);
            float z = radius * sin(phi) * sin(theta);

            file << x << " " << y << " " << z << std::endl;        }
    }
    file.close();   
}