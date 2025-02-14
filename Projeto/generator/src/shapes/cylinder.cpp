#include "../shapes/cylinder.hpp"


// needs t be fixed

void generateCylinder(int radius, int height, int slices, char* outputFile){
    std::string fullPath = "../models/" + std::string(outputFile);
    std::ofstream outFile(fullPath);
    
    if(!outFile.is_open())
    {
        std::cerr << "Error opening outputfile" << outputFile << std::endl;
        return;
    }

    outFile << "cylinder" << "\n";


    float delta = 2 * M_PI / slices;
    float sectorAngle;
    // base: 

    for(int i = 0; i <= slices; i++){
        sectorAngle = i * delta;

        outFile << cos(sectorAngle) << " " << 0.0f << " " << sin(sectorAngle) << "\n";
    }

    // top 
    
    for(int i = 0; i <= slices; i++){
        sectorAngle = i * delta;

        outFile << cos(sectorAngle) << " " << 0.0f << " " << sin(sectorAngle) << "\n";
    }
    
}