#include "../shapes/torus.hpp"

void generateTorus(int minorRadius, int majorRadius, int stacks, int slices, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    float sliceStep = (float) 2 * M_PI / slices;
    float stackStep = (float) 2 * M_PI / stacks;

    float sliceAngle, stackAngle;
    float x, y, z, xy;

    outFile << "torus" << "\n";

    for(int i = 0; i < slices; i++) {
        float sliceAngle1 = M_PI - i * sliceStep;       // Ângulo para o slice atual
        float sliceAngle2 = M_PI - (i + 1) * sliceStep; // Ângulo para o próximo slice

        // Calcula as projeções no plano xy e as alturas z para os dois slices
        float xy1 = minorRadius * cosf(sliceAngle1);
        float z1 = minorRadius * sinf(sliceAngle1);

        float xy2 = minorRadius * cosf(sliceAngle2);
        float z2 = minorRadius * sinf(sliceAngle2);

        for(int j = 0; j < stacks; j++) {
            float stackAngle1 = j * stackStep;       
            float stackAngle2 = (j + 1) * stackStep; 

            float x1 = (majorRadius + xy1) * cosf(stackAngle1);
            float y1 = (majorRadius + xy1) * sinf(stackAngle1);

            float x2 = (majorRadius + xy1) * cosf(stackAngle2);
            float y2 = (majorRadius + xy1) * sinf(stackAngle2);

            float x3 = (majorRadius + xy2) * cosf(stackAngle1);
            float y3 = (majorRadius + xy2) * sinf(stackAngle1);

            float x4 = (majorRadius + xy2) * cosf(stackAngle2);
            float y4 = (majorRadius + xy2) * sinf(stackAngle2);

            outFile << x1 << " " << y1 << " " << z1 << std::endl;
            outFile << x2 << " " << y2 << " " << z1 << std::endl;
            outFile << x3 << " " << y3 << " " << z2 << std::endl;

            outFile << x2 << " " << y2 << " " << z1 << std::endl;
            outFile << x4 << " " << y4 << " " << z2 << std::endl;
            outFile << x3 << " " << y3 << " " << z2 << std::endl;
        }
    }
}