#include "../shapes/cylinder.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

void generateCylinder(int r, int h, int slices, char* outputFile) {
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    if (!outFile.is_open()) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        return;
    }
    float radius = (float)r;
    float height = (float)h;

    outFile << "cylinder" << "\n";

    float delta = 2 * M_PI / slices;

    // Bottom base (normals pointing down)
    for(int i = 0; i < slices; ++i){
        float nx = 0.0f, ny = -1.0f, nz = 0.0f;  // All bottom normals point down
        
        outFile << 0 << " " << -(height / 2) << " " << 0 << " " << nx << " " << ny << " " << nz << " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin((i + 1) * delta) << " " << -(height / 2) << " "
                << radius * cos((i + 1) * delta) << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin(i * delta) << " " << -(height / 2) << " " 
                << radius * cos(i * delta) << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";
    } 

    // Top base (normals pointing up)
    for(int i = 0; i < slices; ++i){
        float nx = 0.0f, ny = 1.0f, nz = 0.0f;  // All top normals point up
        
        outFile << 0 << " " << (height / 2) << " " << 0 << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin(i * delta) << " " << (height / 2) << " " 
                << radius * cos(i * delta) << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin((i + 1) * delta) << " " << (height / 2) << " "
                << radius * cos((i + 1) * delta) << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";
    } 

    // Side body (normals pointing outward)
    for(int i = 0; i < slices; ++i){
        // Calculate normal for this slice (points radially outward)
        float nx = sin(i * delta);
        float nz = cos(i * delta);
        float ny = 0.0f;  // Side normals are horizontal
        
        float nx_next = sin((i + 1) * delta);
        float nz_next = cos((i + 1) * delta);
        
        // First triangle
        outFile << radius * sin((i + 1) * delta) << " " << height / 2.0f << " "
                << radius * cos((i + 1) * delta) << " " << nx_next << " " << ny << " " << nz_next <<  " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin(i * delta) << " " << height / 2.0f << " "
                << radius * cos(i * delta) << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin(i * delta) << " " << -height / 2.0f << " "
                << radius * cos(i * delta) << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";

        // Second triangle
        outFile << radius * sin((i + 1) * delta) << " " << -height / 2.0f << " "
                << radius * cos((i + 1) * delta) << " " << nx_next << " " << ny << " " << nz_next <<  " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin((i + 1) * delta) << " " << height / 2.0f << " "
                << radius * cos((i + 1) * delta) << " " << nx_next << " " << ny << " " << nz_next <<  " " << "0.0" << " " << "0.0" << "\n";
        outFile << radius * sin(i * delta) << " " << -height / 2.0f << " "
                << radius * cos(i * delta) << " " << nx << " " << ny << " " << nz <<  " " << "0.0" << " " << "0.0" << "\n";
    }

    outFile.close();
}
