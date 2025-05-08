#include "../shapes/sphere.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

void generateSphere(int radius, int slices, int stacks, char* outputFile){
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);

    if(!outFile.is_open()) {
        std::cerr << "Error opening outputfile " << outputFile << std::endl;
        return;
    }

    outFile << "sphere\n";

    for (int i = 0; i < slices; ++i) {
        float theta1 = i * M_PI / slices;
        float theta2 = (i + 1) * M_PI / slices;

        for (int j = 0; j < stacks; ++j) {
            float phi1 = j * 2.0f * M_PI / stacks;
            float phi2 = (j + 1) * 2.0f * M_PI / stacks;

            auto writeVertex = [&](float theta, float phi) {
                float x = radius * sin(theta) * sin(phi);
                float y = radius * cos(theta);
                float z = radius * sin(theta) * cos(phi);

                float nx = sin(theta) * sin(phi);
                float ny = cos(theta);
                float nz = sin(theta) * cos(phi);

                float u = phi / (2.0f * M_PI);
                float v = 1.0f - (theta / M_PI);

                outFile << x << " " << y << " " << z << " ";
                outFile << nx << " " << ny << " " << nz << " ";
                outFile << u << " " << v << "\n";
            };

            // Triângulo 1
            writeVertex(theta1, phi1);
            writeVertex(theta2, phi2);
            writeVertex(theta1, phi2);

            // Triângulo 2
            writeVertex(theta1, phi1);
            writeVertex(theta2, phi1);
            writeVertex(theta2, phi2);
        }
    }

    outFile.close();
}

