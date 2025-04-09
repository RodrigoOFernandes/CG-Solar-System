#include "../shapes/patch.hpp"


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
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
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
    std::vector<ControlPoint> controlPoints;
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }
    
    std::ifstream file(fullPathPatch);
    if (!file) {
        std::cerr << "Error opening file: " << fullPathPatch << std::endl;
        return;
    }
    
    size_t num_patches;
    file >> num_patches;
  
    std::vector<std::vector<size_t>> patches(num_patches, std::vector<size_t>(16));
  

    for (size_t i = 0; i < num_patches; ++i) {
        for (size_t j = 0; j < 16; ++j) {
            size_t idx;
            file >> idx;
            file.ignore();
            patches[i][j] = idx;
        }
    }
  
    size_t numberOfControlPoints;
    file >> numberOfControlPoints;
  
    for (size_t i = 0; i < numberOfControlPoints; ++i) {
        float x, y, z;
        file >> x;
        file.ignore();
        file >> y;
        file.ignore();
        file >> z;
        file.ignore();

        controlPoints.push_back(ControlPoint{x, y, z});
    }
  
    file.close();

    for (size_t i = 0; i < num_patches; ++i) {
        std::vector<ControlPoint> patchControlPoints;
        for (size_t j = 0; j < 16; ++j) {
            patchControlPoints.push_back(controlPoints[patches[i][j]]);
        }
      
        // Precompute all points: creating matrix
        std::vector<std::vector<ControlPoint>> precomputedPoints(tesselation + 2, 
                                                               std::vector<ControlPoint>(tesselation + 2));

        for (int u = 0; u < tesselation; ++u) {
            for (int v = 0; v < tesselation; ++v) {
                float u_param = static_cast<float>(u) / tesselation;
                float v_param = static_cast<float>(v) / tesselation;
                precomputedPoints[u][v] = bezierPoint(patchControlPoints, u_param, v_param);
            }
        }
        for (int u = 0; u <= tesselation; u++) {
          for (int v = 0; v <= tesselation; v++) {
              ControlPoint bottom_left  = precomputedPoints[u][v];
              ControlPoint top_left     = precomputedPoints[u][v+1];
              ControlPoint bottom_right = precomputedPoints[u+1][v];
              ControlPoint top_right    = precomputedPoints[u+1][v+1];
      

              outFile << bottom_left.x  << " " << bottom_left.y  << " " << bottom_left.z  << "\n";
              outFile << top_left.x     << " " << top_left.y     << " " << top_left.z     << "\n";
              outFile << bottom_right.x << " " << bottom_right.y << " " << bottom_right.z << "\n";
      
              outFile << top_left.x     << " " << top_left.y     << " " << top_left.z     << "\n";
              outFile << top_right.x    << " " << top_right.y    << " " << top_right.z    << "\n";
              outFile << bottom_right.x << " " << bottom_right.y << " " << bottom_right.z << "\n";
          }
      }
    }
}