#include "../shapes/patch.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct ControlPoint {
    float x, y, z;
};

void generatePatch(char *patch_file, int tesselation, char *outputFile) {
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

    std::cout << "Number of patches: " << numPatches << std::endl;
    for (int i = 0; i < patches.size(); i++) {
        std::cout << "Patch " << i << " control points:" << std::endl;
        for (int index : patches[i]) {
            std::cout << "  Index " << index << ": ("
                      << controlPoints[index].x << ", "
                      << controlPoints[index].y << ", "
                      << controlPoints[index].z << ")" << std::endl;
        }
    }
    
    std::cout << "Total number of control points: " << numControlPoints << std::endl;
}

