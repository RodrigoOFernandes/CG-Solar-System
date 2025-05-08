#include "../shapes/torus.hpp"

void generateTorus(int minorRadius, int majorRadius, int stacks, int slices, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    float sliceStep = (float) 2 * M_PI / slices;
    float stackStep = (float) 2 * M_PI / stacks;

    outFile << "torus" << "\n";

    for(int i = 0; i < slices; i++) {
        float sliceAngle1 = i * sliceStep;       
        float sliceAngle2 = (i + 1) * sliceStep; 

        for(int j = 0; j < stacks; j++) {
            float stackAngle1 = j * stackStep;       
            float stackAngle2 = (j + 1) * stackStep; 

            float x1 = (majorRadius + minorRadius * cosf(sliceAngle1)) * cosf(stackAngle1);
            float y1 = (majorRadius + minorRadius * cosf(sliceAngle1)) * sinf(stackAngle1);
            float z1 = minorRadius * sinf(sliceAngle1);

            float x2 = (majorRadius + minorRadius * cosf(sliceAngle1)) * cosf(stackAngle2);
            float y2 = (majorRadius + minorRadius * cosf(sliceAngle1)) * sinf(stackAngle2);
            float z2 = minorRadius * sinf(sliceAngle1);

            float x3 = (majorRadius + minorRadius * cosf(sliceAngle2)) * cosf(stackAngle1);
            float y3 = (majorRadius + minorRadius * cosf(sliceAngle2)) * sinf(stackAngle1);
            float z3 = minorRadius * sinf(sliceAngle2);

            float x4 = (majorRadius + minorRadius * cosf(sliceAngle2)) * cosf(stackAngle2);
            float y4 = (majorRadius + minorRadius * cosf(sliceAngle2)) * sinf(stackAngle2);
            float z4 = minorRadius * sinf(sliceAngle2);

            outFile << x1 << " " << y1 << " " << z1 << std::endl;
            outFile << x2 << " " << y2 << " " << z2 << std::endl;
            outFile << x3 << " " << y3 << " " << z3 << std::endl;

            outFile << x2 << " " << y2 << " " << z2 << std::endl;
            outFile << x4 << " " << y4 << " " << z4 << std::endl;
            outFile << x3 << " " << y3 << " " << z3 << std::endl;
        }
    }
}
