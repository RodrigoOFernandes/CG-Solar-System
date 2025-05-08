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
    float texStep = 1.0f / divisions;

    for (int face = 0; face < 6; face++) {
        float nx = 0, ny = 0, nz = 0;

        switch (face) {
            case 0: nz = 1; break;  // Frente
            case 1: nz = -1; break; // Trás
            case 2: nx = -1; break; // Esquerda
            case 3: nx = 1; break;  // Direita
            case 4: ny = 1; break;  // Topo
            case 5: ny = -1; break; // Base
        }

        for (int i = 0; i < divisions; i++) {
            for (int j = 0; j < divisions; j++) {
                float x1 = -halfDim + j * step;
                float x2 = x1 + step;
                float y1 = -halfDim + i * step;
                float y2 = y1 + step;

                float tx1 = j * texStep;
                float tx2 = tx1 + texStep;
                float ty1 = i * texStep;
                float ty2 = ty1 + texStep;

                // Os vértices mudam conforme a face, por isso calculamos o valor correto de x/y/z
                auto emit = [&](float x, float y, float z, float tx, float ty) {
                    outFile << x << " " << y << " " << z << " "
                            << nx << " " << ny << " " << nz << " "
                            << tx << " " << ty << "\n";
                };

                switch (face) {
                    case 0: // Frente (z fixo)
                        emit(x1, y2, halfDim, tx1, ty2);
                        emit(x1, y1, halfDim, tx1, ty1);
                        emit(x2, y2, halfDim, tx2, ty2);

                        emit(x2, y2, halfDim, tx2, ty2);
                        emit(x1, y1, halfDim, tx1, ty1);
                        emit(x2, y1, halfDim, tx2, ty1);
                        break;

                    case 1: // Trás
                        emit(x1, y2, -halfDim, tx2, ty2);
                        emit(x2, y2, -halfDim, tx1, ty2);
                        emit(x1, y1, -halfDim, tx2, ty1);

                        emit(x2, y2, -halfDim, tx1, ty2);
                        emit(x2, y1, -halfDim, tx1, ty1);
                        emit(x1, y1, -halfDim, tx2, ty1);
                        break;

                    case 2: // Esquerda (x fixo)
                        emit(-halfDim, y2, x1, tx2, ty2);
                        emit(-halfDim, y1, x1, tx2, ty1);
                        emit(-halfDim, y2, x2, tx1, ty2);

                        emit(-halfDim, y2, x2, tx1, ty2);
                        emit(-halfDim, y1, x1, tx2, ty1);
                        emit(-halfDim, y1, x2, tx1, ty1);
                        break;

                    case 3: // Direita
                        emit(halfDim, y2, x1, tx1, ty2);
                        emit(halfDim, y2, x2, tx2, ty2);
                        emit(halfDim, y1, x1, tx1, ty1);

                        emit(halfDim, y2, x2, tx2, ty2);
                        emit(halfDim, y1, x2, tx2, ty1);
                        emit(halfDim, y1, x1, tx1, ty1);
                        break;

                    case 4: // Topo (y fixo)
                        emit(x1, halfDim, y2, tx1, ty2);
                        emit(x2, halfDim, y2, tx2, ty2);
                        emit(x1, halfDim, y1, tx1, ty1);

                        emit(x2, halfDim, y2, tx2, ty2);
                        emit(x2, halfDim, y1, tx2, ty1);
                        emit(x1, halfDim, y1, tx1, ty1);
                        break;

                    case 5: // Base
                        emit(x1, -halfDim, y2, tx1, ty2);
                        emit(x1, -halfDim, y1, tx1, ty1);
                        emit(x2, -halfDim, y2, tx2, ty2);

                        emit(x2, -halfDim, y2, tx2, ty2);
                        emit(x1, -halfDim, y1, tx1, ty1);
                        emit(x2, -halfDim, y1, tx2, ty1);
                        break;
                }
            }
        }
    }

    outFile.close();
}
