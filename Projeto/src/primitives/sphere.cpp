#include "include/primitives/sphere.h"


void generateSphere(float raio, int slices, int stacks, char* outputFile)
{
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
            float x = raio * sin(phi) * cos(theta);
            float y = raio * cos(phi);
            float z = raio * sin(phi) * sin(theta);

            file << x << " " << y << " " << z << std::endl;        }
    }
    file.close();   
}