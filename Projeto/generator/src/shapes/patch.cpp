#include "../shapes/patch.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

struct ControlPoint {
    float x, y, z;
};

ControlPoint bezierPoint(const std::vector<ControlPoint>& controlPoints, float u, float v) {
    ControlPoint result = {0, 0, 0};
    float B[4];
    for (int i = 0; i < 4; i++) {
        B[i] = (1 - u) * (1 - u) * (1 - u) * (i == 0) +
               3 * u * (1 - u) * (1 - u) * (i == 1) +
               3 * u * u * (1 - u) * (i == 2) +
               u * u * u * (i == 3);
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float coef = B[i] * B[j];
            int index = i * 4 + j;
            result.x += coef * controlPoints[index].x;
            result.y += coef * controlPoints[index].y;
            result.z += coef * controlPoints[index].z;
        }
    }
    return result;
}

void generatePatch(char *patch_file, int tesselation, char *outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }

    std::ifstream file(patch_file);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    int numPatches;
    file >> numPatches;
    std::vector<std::vector<int>> patches(numPatches);
    
    for (int i = 0; i < numPatches; i++) {
        std::string line;
        std::getline(file >> std::ws, line);
        std::istringstream iss(line);
        int index;
        while (iss >> index) {
            patches[i].push_back(index);
            if (iss.peek() == ',') iss.ignore();
        }
    }

    int numControlPoints;
    file >> numControlPoints;
    std::vector<ControlPoint> controlPoints(numControlPoints);
    
    for (int i = 0; i < numControlPoints; i++) {
        char comma;
        file >> controlPoints[i].x >> comma >> controlPoints[i].y >> comma >> controlPoints[i].z;
    }

    for (const auto& patch : patches) {
        std::vector<ControlPoint> patchPoints(16);
        for (int i = 0; i < 16; i++) {
            patchPoints[i] = controlPoints[patch[i]];
        }
        
        for (int i = 0; i < tesselation; i++) {
            float u1 = (float)i / tesselation;
            float u2 = (float)(i + 1) / tesselation;
            for (int j = 0; j < tesselation; j++) {
                float v1 = (float)j / tesselation;
                float v2 = (float)(j + 1) / tesselation;
                
                ControlPoint p1 = bezierPoint(patchPoints, u1, v1);
                ControlPoint p2 = bezierPoint(patchPoints, u2, v1);
                ControlPoint p3 = bezierPoint(patchPoints, u1, v2);
                ControlPoint p4 = bezierPoint(patchPoints, u2, v2);
                
                outFile << p1.x << " " << p1.y << " " << p1.z << "\n";
                outFile << p2.x << " " << p2.y << " " << p2.z << "\n";
                outFile << p3.x << " " << p3.y << " " << p3.z << "\n";
                outFile << p2.x << " " << p2.y << " " << p2.z << "\n";
                outFile << p4.x << " " << p4.y << " " << p4.z << "\n";
                outFile << p3.x << " " << p3.y << " " << p3.z << "\n";
            }
        }
    }
}

