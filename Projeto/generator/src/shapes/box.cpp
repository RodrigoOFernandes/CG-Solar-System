#include "../shapes/box.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

void generateBox(int dimension, int divisions, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);

    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return;
    }

    outFile << "box" << "\n";

    float halfDim = dimension / 2.0f;
    float step = static_cast<float>(dimension) / divisions;

    for (int face = 0; face < 6; face++) {
        float nx = 0.0f, ny = 0.0f, nz = 0.0f;
        
        switch (face) {
            case 0: nx = 0.0f;  ny = 0.0f;  nz = 1.0f;  break;  // Front (positive Z)
            case 1: nx = 0.0f;  ny = 0.0f;  nz = -1.0f; break;  // Back (negative Z)
            case 2: nx = -1.0f; ny = 0.0f;  nz = 0.0f;  break;  // Left (negative X)
            case 3: nx = 1.0f;  ny = 0.0f;  nz = 0.0f;  break;  // Right (positive X)
            case 4: nx = 0.0f;  ny = 1.0f;  nz = 0.0f;  break;  // Top (positive Y)
            case 5: nx = 0.0f;  ny = -1.0f; nz = 0.0f;  break;  // Bottom (negative Y)
        }

        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                float x1 = -halfDim + j * step;
                float x2 = x1 + step;
                float y1 = -halfDim + i * step;
                float y2 = y1 + step;

                switch (face) {
                    case 0: // Frente 
                        outFile << x1 << " " << y2 << " " << halfDim << " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << y1 << " " << halfDim << " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << y2 << " " << halfDim << " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";

                        outFile << x2 << " " << y2 << " " << halfDim << " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << y1 << " " << halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << y1 << " " << halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        break;
                    case 1: // Trás 
                        outFile << x1 << " " << y2 << " " << -halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << y2 << " " << -halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << y1 << " " << -halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";

                        outFile << x2 << " " << y2 << " " << -halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << y1 << " " << -halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << y1 << " " << -halfDim <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        break;
                    case 2: // Esquerda 
                        outFile << -halfDim << " " << y2 << " " << x1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << -halfDim << " " << y1 << " " << x1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << -halfDim << " " << y2 << " " << x2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";

                        outFile << -halfDim << " " << y2 << " " << x2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << -halfDim << " " << y1 << " " << x1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << -halfDim << " " << y1 << " " << x2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        break;
                    case 3: // Direita 
                        outFile << halfDim << " " << y2 << " " << x1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << halfDim << " " << y2 << " " << x2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << halfDim << " " << y1 << " " << x1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";

                        outFile << halfDim << " " << y2 << " " << x2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << halfDim << " " << y1 << " " << x2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << halfDim << " " << y1 << " " << x1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        break;
                    case 4: // Topo 
                        outFile << x1 << " " << halfDim << " " << y2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << halfDim << " " << y2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << halfDim << " " << y1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";

                        outFile << x2 << " " << halfDim << " " << y2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << halfDim << " " << y1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << halfDim << " " << y1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        break;
                    case 5: // Base
                        outFile << x1 << " " << -halfDim << " " << y2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << -halfDim << " " << y1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << -halfDim << " " << y2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";

                        outFile << x2 << " " << -halfDim << " " << y2 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x1 << " " << -halfDim << " " << y1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        outFile << x2 << " " << -halfDim << " " << y1 <<  " " << nx << " " << ny << " " << nz << " " << 0.0 << " " << 0.0 << "\n";
                        break;
                }
            }
        }
    }
    outFile.close();
}

