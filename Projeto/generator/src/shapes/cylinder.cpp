#include "../shapes/cylinder.hpp"


void generateCylinder(int r, int h , int slices, char* outputFile) {
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

    //base
    for(int i = 0; i < slices; ++i){
        outFile << 0 << " " << -(height / 2) << " " << 0 << "\n";
        outFile << radius * sin((i + 1) * delta) << " " << -(height / 2) << " "
                            << radius * cos((i + 1) * delta)  << "\n";
        outFile << radius * sin(i * delta) << " " << -(height / 2) << " " 
                            << radius * cos(i * delta) << "\n";
        
    } 

    // Top
    for(int i = 0; i < slices; ++i){
        outFile << 0 << " " << (height / 2) << " " << 0 << "\n";
        outFile << radius * sin(i * delta) << " " << (height / 2) << " " 
                            << radius * cos(i * delta) << "\n";
        outFile << radius * sin((i + 1) * delta) << " " << (height / 2) << " "
                            << radius * cos((i + 1) * delta)  << "\n";

        
    } 

    //corpo

    for(int i = 0; i < slices; ++i){

        outFile << radius * sin((i + 1) * delta) << " " << height / 2.0f << " "
                            << radius * cos((i + 1) * delta) << "\n";
        outFile << radius * sin(i * delta) << " " << height / 2.0f << " "
                            << radius * cos(i * delta) << "\n";
        outFile << radius * sin(i * delta) << " " << -height / 2.0f << " "
                            << radius * cos(i * delta) << "\n";


        outFile << radius * sin((i + 1) * delta) << " " << -height / 2.0f << " "
                            << radius * cos((i + 1) * delta) << "\n";
        outFile << radius * sin((i + 1) * delta) << " " << height / 2.0f << " "
                            << radius * cos((i + 1) * delta) << "\n";
        outFile << radius * sin(i * delta) << " " << -height / 2.0f << " "
                            << radius * cos(i * delta) << "\n";
    }




}