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
        
        std::vector<int> patchIndices;
        int index;
        while (patchIndices.size() < 16 && iss >> index) {
            patchIndices.push_back(index);
            if (iss.peek() == ',') iss.ignore();
        }
        
        if (patchIndices.size() != 16) {
            std::cerr << "Error: Patch " << i << " does not have 16 indices." << std::endl;
            continue;
        }
        
        patches[i] = patchIndices;
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
            if (patch[i] < 0 || patch[i] >= numControlPoints) {
                std::cerr << "Invalid control point index: " << patch[i] << std::endl;
                continue;
            }
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
        
        // Generate triangles following right-hand rule (single triangle per square)
        for (int i = 0; i < tesselation ; i++) {
            for (int j = 0; j < tesselation ; j++) {
                // Get four corner points of the current grid square
                ControlPoint p1 = surfacePoints[i][j];      // Bottom-left
                ControlPoint p2 = surfacePoints[i+1][j];    // Top-left
                ControlPoint p3 = surfacePoints[i][j+1];    // Bottom-right
                ControlPoint p4 = surfacePoints[i+1][j+1];  // Top-right
                
                // First triangle: (p1, p2, p3)
                outFile << p1.x << " " << p1.y << " " << p1.z << "\n";
                outFile << p4.x << " " << p4.y << " " << p4.z << "\n";
                 outFile << p2.x << " " << p2.y << " " << p2.z << "\n";
                outFile << p1.x << " " << p1.y << " " << p1.z << "\n";
                 outFile << p3.x << " " << p3.y << " " << p3.z << "\n";
                outFile << p4.x << " " << p4.y << " " << p4.z << "\n"; 
            }
        }
    }
    
    outFile.close();
}
