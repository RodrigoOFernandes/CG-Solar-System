#include "../shapes/torus.hpp"

void generateTorus(int minorRadius, int majorRadius, int stacks, int slices, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    float sliceStep = (float) 2 * M_PI / slices;
    float stackStep = (float) 2 * M_PI / stacks;

    float sliceAngle, stackAngle;
    float x, y, z, xy;

    for(int i = 0; i <= slices; i++)
    {
        sliceAngle = M_PI - i * sliceStep;
        
        xy = minorRadius * cosf(sliceAngle);
        z = minorRadius * sinf(sliceAngle);

        for(int j = 0; j <= stacks; j++)
        {
            stackAngle = j * stackStep;

            x = xy * cosf(stackAngle);
            y = xy * sinf(stackAngle);

            x += majorRadius * cosf(stackAngle);
            y += majorRadius * sinf(stackAngle);

            outFile << x << " " << y << " " << z << std::endl;
        }   
    }
}