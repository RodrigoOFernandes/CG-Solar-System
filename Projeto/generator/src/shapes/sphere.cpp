#include "../shapes/sphere.hpp"


void generateSphere(int radius, int slices, int stacks, char* outputFile){
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    if(!outFile.is_open())
    {
        std::cerr << "Error opening outputfile" << outputFile << std::endl;
        return;
    }

    outFile << "sphere" << "\n";

    for (int i = 0; i < slices; ++i) {
        float theta1 = static_cast<float>(i) * M_PI / static_cast<float>(stacks);
        float theta2 = static_cast<float>(i + 1) * M_PI / static_cast<float>(stacks);


        for (int j = 0; j < stacks; ++j) {
        float phi1 = static_cast<float>(j) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(stacks);
        float phi2 = static_cast<float>(j + 1) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(stacks);

            float z1 = radius * std::sin(theta1) * std::cos(phi1);
            float x1 = radius * std::sin(theta1) * std::sin(phi1);
            float y1 = radius * std::cos(theta1);

            float z2 = radius * std::sin(theta1) * std::cos(phi2);
            float x2 = radius * std::sin(theta1) * std::sin(phi2);
            float y2 = radius * std::cos(theta1);

            float z3 = radius * std::sin(theta2) * std::cos(phi1);
            float x3 = radius * std::sin(theta2) * std::sin(phi1);
            float y3 = radius * std::cos(theta2);

            float z4 = radius * std::sin(theta2) * std::cos(phi2);
            float x4 = radius * std::sin(theta2) * std::sin(phi2);
            float y4 = radius * std::cos(theta2);          

            outFile << x1 << " " << y1 << " " << z1 << std::endl;
            outFile << x4 << " " << y4 << " " << z4 << std::endl;
            outFile << x2 << " " << y2 << " " << z2 << std::endl;
            
            outFile << x1 << " " << y1 << " " << z1 << std::endl;
            outFile << x3 << " " << y3 << " " << z3 << std::endl;
            outFile << x4 << " " << y4 << " " << z4 << std::endl;
        }
    }
    outFile.close();   
}