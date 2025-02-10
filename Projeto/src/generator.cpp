#include "include/generator.h"

int main(int argc, char** argv){
    if(argc < 2)
    {
        printf("Usage: generator <primitive> <params> <file>");
    }
    
    char* primitiva = argv[1];
    char* outputFile = argv[argc - 1];

    if(strcmp(primitiva, SPHERE) == 0){
        float radius = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        generateSphere(radius, slices, stacks, outputFile);
    }

    return 0;
}