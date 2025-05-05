#include "../shapes/torus.hpp"
#include <cmath>
#include <fstream>
#include <string>

void generateTorus(int minorRadius, int majorRadius, int stacks, int slices, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    float sliceStep = (float) 2 * M_PI / slices;
    float stackStep = (float) 2 * M_PI / stacks;

    // Calculate total number of triangles (2 per quad, per stack and slice)
    int totalTriangles = 2 * slices * stacks;
    outFile << totalTriangles << "\n";

    for(int i = 0; i < slices; i++) {
        float sliceAngle1 = i * sliceStep;       
        float sliceAngle2 = (i + 1) * sliceStep; 

        for(int j = 0; j < stacks; j++) {
            float stackAngle1 = j * stackStep;       
            float stackAngle2 = (j + 1) * stackStep; 

            // Calculate vertices and normals for first triangle
            float x1 = (majorRadius + minorRadius * cosf(sliceAngle1)) * cosf(stackAngle1);
            float y1 = (majorRadius + minorRadius * cosf(sliceAngle1)) * sinf(stackAngle1);
            float z1 = minorRadius * sinf(sliceAngle1);
            // Normal for first vertex
            float nx1 = cosf(sliceAngle1) * cosf(stackAngle1);
            float ny1 = cosf(sliceAngle1) * sinf(stackAngle1);
            float nz1 = sinf(sliceAngle1);

            float x2 = (majorRadius + minorRadius * cosf(sliceAngle1)) * cosf(stackAngle2);
            float y2 = (majorRadius + minorRadius * cosf(sliceAngle1)) * sinf(stackAngle2);
            float z2 = minorRadius * sinf(sliceAngle1);
            // Normal for second vertex (same slice angle as first vertex)
            float nx2 = nx1;
            float ny2 = ny1;
            float nz2 = nz1;

            float x3 = (majorRadius + minorRadius * cosf(sliceAngle2)) * cosf(stackAngle1);
            float y3 = (majorRadius + minorRadius * cosf(sliceAngle2)) * sinf(stackAngle1);
            float z3 = minorRadius * sinf(sliceAngle2);
            // Normal for third vertex
            float nx3 = cosf(sliceAngle2) * cosf(stackAngle1);
            float ny3 = cosf(sliceAngle2) * sinf(stackAngle1);
            float nz3 = sinf(sliceAngle2);

            // Calculate vertices and normals for second triangle
            float x4 = (majorRadius + minorRadius * cosf(sliceAngle2)) * cosf(stackAngle2);
            float y4 = (majorRadius + minorRadius * cosf(sliceAngle2)) * sinf(stackAngle2);
            float z4 = minorRadius * sinf(sliceAngle2);
            // Normal for fourth vertex
            float nx4 = cosf(sliceAngle2) * cosf(stackAngle2);
            float ny4 = cosf(sliceAngle2) * sinf(stackAngle2);
            float nz4 = sinf(sliceAngle2);

            // First triangle (v1, v2, v3)
            outFile << x1 << " " << y1 << " " << z1 << " "
                   << nx1 << " " << ny1 << " " << nz1 << " 0.0 0.0\n";
            outFile << x2 << " " << y2 << " " << z2 << " "
                   << nx2 << " " << ny2 << " " << nz2 << " 0.0 0.0\n";
            outFile << x3 << " " << y3 << " " << z3 << " "
                   << nx3 << " " << ny3 << " " << nz3 << " 0.0 0.0\n";

            // Second triangle (v2, v4, v3)
            outFile << x2 << " " << y2 << " " << z2 << " "
                   << nx2 << " " << ny2 << " " << nz2 << " 0.0 0.0\n";
            outFile << x4 << " " << y4 << " " << z4 << " "
                   << nx4 << " " << ny4 << " " << nz4 << " 0.0 0.0\n";
            outFile << x3 << " " << y3 << " " << z3 << " "
                   << nx3 << " " << ny3 << " " << nz3 << " 0.0 0.0\n";
        }
    }
    outFile.close();
}
