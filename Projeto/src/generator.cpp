#include "include/generator.h"

int main(int argc, char** argv){
    if(argc < 2)
    {
        printf("Uso: generator <primitiva> <params> <ficheiro>");
    }
    
    char* primitiva = argv[1];
    char* outputFile = argv[argc - 1];

    if(strcmp(primitiva, SPHERE) == 0){
        float raio = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        generateSphere(raio, slices, stacks, outputFile);
    }

    return 0;
}