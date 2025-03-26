#include "../shapes/patch.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>

struct ControlPoint {
    float x, y, z;
};

// Compute Bernstein polynomial
float bernstein(int i, int n, float t) {
    float result = 1.0f;
    for (int j = 1; j <= i; ++j) {
        result *= (n - j + 1.0f) / j;
    }
    return result * std::pow(t, i) * std::pow(1 - t, n - i);
}

// Compute a point on a Bézier surface using more efficient algorithm
ControlPoint bezierPoint(const std::vector<ControlPoint>& patch, float u, float v) {
    ControlPoint p = {0, 0, 0};
    for (int i = 0; i < 4; i++) {
        float bu = bernstein(i, 3, u);
        for (int j = 0; j < 4; j++) {
            float bv = bernstein(j, 3, v);
            float b = bu * bv;
            
            p.x += patch[i * 4 + j].x * b;
            p.y += patch[i * 4 + j].y * b;
            p.z += patch[i * 4 + j].z * b;
        }
    }
    return p;
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
        
        // Improved tessellation approach with fewer triangles
        std::vector<ControlPoint> surfacePoints((tesselation + 1) * (tesselation + 1));
        
        // Precompute surface points
        for (int i = 0; i <= tesselation; i++) {
            float u = (float)i / tesselation;
            for (int j = 0; j <= tesselation; j++) {
                float v = (float)j / tesselation;
                surfacePoints[i * (tesselation + 1) + j] = bezierPoint(patchPoints, u, v);
            }
        }
        
        // Generate triangles more efficiently
        for (int i = 0; i < tesselation; i++) {
            for (int j = 0; j < tesselation; j++) {
                int idx00 = i * (tesselation + 1) + j;
                int idx10 = (i + 1) * (tesselation + 1) + j;
                int idx01 = i * (tesselation + 1) + (j + 1);
                int idx11 = (i + 1) * (tesselation + 1) + (j + 1);
                
                // First triangle
                outFile << surfacePoints[idx10].x << " " << surfacePoints[idx10].y << " " << surfacePoints[idx10].z << "\n";
                outFile << surfacePoints[idx01].x << " " << surfacePoints[idx01].y << " " << surfacePoints[idx01].z << "\n";
                outFile << surfacePoints[idx00].x << " " << surfacePoints[idx00].y << " " << surfacePoints[idx00].z << "\n";
                
                // Second triangle
                outFile << surfacePoints[idx11].x << " " << surfacePoints[idx11].y << " " << surfacePoints[idx11].z << "\n";
                outFile << surfacePoints[idx01].x << " " << surfacePoints[idx01].y << " " << surfacePoints[idx01].z << "\n";
                outFile << surfacePoints[idx10].x << " " << surfacePoints[idx10].y << " " << surfacePoints[idx10].z << "\n";
            }
        }
    }
    
    outFile.close();
}
