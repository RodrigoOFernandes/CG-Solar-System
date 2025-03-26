#include "../shapes/patch.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

struct ControlPoint {
    float x, y, z;
};

float bernstein(int i, float t) {
    switch (i) {
        case 0:
            return std::pow(1 - t, 3);
        case 1:
            return 3 * t * std::pow(1 - t, 2);
        case 2:
            return 3 * std::pow(t, 2) * (1 - t);
        case 3:
            return std::pow(t, 3);
        default:
            return 0;
    }
}

ControlPoint bezierPoint(const std::vector<ControlPoint>& patch, float u, float v) {
    ControlPoint p = {0, 0, 0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            float b = bernstein(i, u) * bernstein(j, v);
            
            p.x += patch[i * 4 + j].x * b;
            p.y += patch[i * 4 + j].y * b;
            p.z += patch[i * 4 + j].z * b;
        }
    }
    return p;
}

void generatePatch(char *patch_file, int tesselation, char *outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::string fullPathPatch = "../patches/" + std::string(patch_file);
    std::ofstream outFile(fullPath);
    
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }
    
    std::ifstream file(fullPathPatch);
    if (!file) {
        std::cerr << "Error opening file: " << fullPathPatch << std::endl;
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
        
        // Create points for the current tessellation level
        std::vector<std::vector<ControlPoint>> surfacePoints(tesselation + 1, 
            std::vector<ControlPoint>(tesselation + 1));
        
        // Precompute surface points
        for (int i = 0; i <= tesselation; i++) {
            float u = (float)i / tesselation;
            for (int j = 0; j <= tesselation; j++) {
                float v = (float)j / tesselation;
                surfacePoints[i][j] = bezierPoint(patchPoints, u, v);
            }
        }
        
        // Generate triangles
        for (int i = 0; i < tesselation; i++) {
            for (int j = 0; j < tesselation; j++) {
                // First triangle
                outFile << surfacePoints[i][j].x << " " << surfacePoints[i][j].y << " " << surfacePoints[i][j].z << "\n";
                outFile << surfacePoints[i+1][j+1].x << " " << surfacePoints[i+1][j+1].y << " " << surfacePoints[i+1][j+1].z << "\n";
                outFile << surfacePoints[i+1][j].x << " " << surfacePoints[i+1][j].y << " " << surfacePoints[i+1][j].z << "\n";
                
                // Second triangle
                outFile << surfacePoints[i][j].x << " " << surfacePoints[i][j].y << " " << surfacePoints[i][j].z << "\n";
                outFile << surfacePoints[i][j+1].x << " " << surfacePoints[i][j+1].y << " " << surfacePoints[i][j+1].z << "\n";
                outFile << surfacePoints[i+1][j+1].x << " " << surfacePoints[i+1][j+1].y << " " << surfacePoints[i+1][j+1].z << "\n";
            }
        }
    }
    
    outFile.close();
}
