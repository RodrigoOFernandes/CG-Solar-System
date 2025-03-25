#include "../shapes/patch.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct ControlPoint {
    float x, y, z;
};

void parsePatch(char *patch_file) {
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
            // Adjust for 0-based indexing if needed
            patches[i].push_back(index);  // Or use (index - 1) if file uses 1-based indexing
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

    // Debug output
    std::cout << "Number of patches: " << numPatches << std::endl;
    for (const auto& patch : patches) {
        for (int index : patch) {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "Number of control points: " << numControlPoints << std::endl;
    for (int i = 0; i < numControlPoints - 1; i++) {
        std::cout << "Control Point " << i << ": "
                  << controlPoints[i].x << ", "
                  << controlPoints[i].y << ", "
                  << controlPoints[i].z << std::endl;
    }  
}

void generatePatch(char *patch_file, int tesselation, char *outputFile) {
   parsePatch(patch_file);
}

