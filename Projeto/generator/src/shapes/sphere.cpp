#include "../shapes/sphere.hpp"
#include <cmath>
#include <fstream>
#include <iostream>

void generateSphere(int radius, int slices, int stacks, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    if(!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }

    outFile << "sphere" << "\n";

    for (int i = 0; i < slices; ++i) {
        float theta1 = static_cast<float>(i) * M_PI / static_cast<float>(slices);
        float theta2 = static_cast<float>(i + 1) * M_PI / static_cast<float>(slices);

        for (int j = 0; j < stacks; ++j) {
            float phi1 = static_cast<float>(j) * 2.0f * M_PI / static_cast<float>(stacks);
            float phi2 = static_cast<float>(j + 1) * 2.0f * M_PI / static_cast<float>(stacks);

            // Calculate vertex positions
            float x1 = radius * std::sin(theta1) * std::sin(phi1);
            float y1 = radius * std::cos(theta1);
            float z1 = radius * std::sin(theta1) * std::cos(phi1);

            float x2 = radius * std::sin(theta1) * std::sin(phi2);
            float y2 = radius * std::cos(theta1);
            float z2 = radius * std::sin(theta1) * std::cos(phi2);

            float x3 = radius * std::sin(theta2) * std::sin(phi1);
            float y3 = radius * std::cos(theta2);
            float z3 = radius * std::sin(theta2) * std::cos(phi1);

            float x4 = radius * std::sin(theta2) * std::sin(phi2);
            float y4 = radius * std::cos(theta2);
            float z4 = radius * std::sin(theta2) * std::cos(phi2);

            // Calculate normals (normalized vertex positions)
            float nx1 = x1 / radius;
            float ny1 = y1 / radius;
            float nz1 = z1 / radius;

            float nx2 = x2 / radius;
            float ny2 = y2 / radius;
            float nz2 = z2 / radius;

            float nx3 = x3 / radius;
            float ny3 = y3 / radius;
            float nz3 = z3 / radius;

            float nx4 = x4 / radius;
            float ny4 = y4 / radius;
            float nz4 = z4 / radius;

            // First triangle
            outFile << x1 << " " << y1 << " " << z1 << " " << nx1 << " " << ny1 << " " << nz1 << " " << "0.0" << " " << "0.0" << std::endl;
            outFile << x4 << " " << y4 << " " << z4 << " " << nx4 << " " << ny4 << " " << nz4 <<  " " << "0.0" << " " << "0.0" << std::endl;
            outFile << x2 << " " << y2 << " " << z2 << " " << nx2 << " " << ny2 << " " << nz2 <<  " " << "0.0" << " " << "0.0" << std::endl;
            
            // Second triangle
            outFile << x1 << " " << y1 << " " << z1 << " " << nx1 << " " << ny1 << " " << nz1 <<  " " << "0.0" << " " << "0.0" << std::endl;
            outFile << x3 << " " << y3 << " " << z3 << " " << nx3 << " " << ny3 << " " << nz3 <<  " " << "0.0" << " " << "0.0" << std::endl;
            outFile << x4 << " " << y4 << " " << z4 << " " << nx4 << " " << ny4 << " " << nz4 <<  " " << "0.0" << " " << "0.0" << std::endl;

        }
    }
    outFile.close();   
}
