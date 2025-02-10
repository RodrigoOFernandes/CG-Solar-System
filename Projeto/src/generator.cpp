#include "include/generator.h"

int main(int argc, char** argv){
    if(argc < 2)
    {
        printf("Usage: generator <primitive> <params> <file>");
    }
    
    char* primitive = argv[1];
    char* outputFile = argv[argc - 1];

    if(strcmp(primitive, SPHERE) == 0){
        float radius = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        generateSphere(radius, slices, stacks, outputFile);
    } else if(strcmp(primitive, PLANE) == 0){
        printf("plane not available\n");
    } else if(strcmp(primitive, BOX) == 0){
        printf("box not available\n");
    } else if(strcmp(primitive, CONE) == 0){
        printf("cone not available\n");
    }

    return 0;
}