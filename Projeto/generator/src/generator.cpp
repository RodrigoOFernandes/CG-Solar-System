#include "generator.hpp"

int main(int argc, char** argv){
    if(argc < 2)
    {
        printf("Usage: generator <primitive> <params> <file>");
    }
    
    char* primitive = argv[1];
    char* outputFile = argv[argc - 1];

    if(strcmp(primitive, SPHERE) == 0){
        int radius = std::stoi(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        generateSphere(radius, slices, stacks, outputFile);
    } else if(strcmp(primitive, PLANE) == 0){
        int length = std::stoi(argv[2]);
        int divisions = std::stoi(argv[3]);
        generatePlane(length, divisions, outputFile);
    } else if(strcmp(primitive, BOX) == 0){
        int length = std::stoi(argv[2]);
        int divisions = std::stof(argv[3]);
        generateBox(length,divisions,outputFile);
    } else if(strcmp(primitive, CONE) == 0){
        int radius = std::stof(argv[2]);
        int height = std::stof(argv[3]);
        int slices = std::stof(argv[4]);
        int stacks = std::stof(argv[5]);
        generateCone(radius, height, slices, stacks, outputFile);
    } else if(strcmp(primitive, CYLINDER) == 0){
        int radius = std::stoi(argv[2]);
        int height = std::stoi(argv[3]);
        int slices = std::stoi(argv[4]);
        generateCylinder(radius, height, slices, outputFile);
    } else if(strcmp(primitive, TORUS) == 0) {
        int minor_radius = std::stoi(argv[2]);
        int major_radius = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        int slices = std::stoi(argv[5]);
        generateTorus(minor_radius, major_radius, stacks, slices, outputFile);
    } else if(strcmp(primitive, PATCH) == 0) {
        char *patch_file = argv[2];
        int tesselation = std::stoi(argv[3]);
        generatePatch(patch_file, tesselation, outputFile);
    }

    return 0;
}
