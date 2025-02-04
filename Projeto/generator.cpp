#include <stdio.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cmath>

void geraEsfera(float raio, int slices, int stacks, char* outputFile)
{
    std::ofstream file(outputFile);

    if(!file.is_open())
    {
        std::cerr << "Erro ao abrir ficheiro de output" << outputFile << std::endl;
        return;
    }

    file << (slices + 1) * (stacks + 1) << std::endl;

    for(int i = 0; i <= stacks; i++)
    {
        float phi =  M_PI * i / stacks;
        for(int j = 0; j <= slices; j++)
        {
            float theta =  2 * M_PI * j / slices;
            float x = raio * sin(phi) * cos(theta);
            float y = raio * cos(phi);
            float z = raio * sin(phi) * sin(theta);

            file << x << " " << y << " " << z << std::endl;        }
    }
    file.close();   
}

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Uso: generator <primitiva> <params> <ficheiro>");
    }
    
    char* primitiva = argv[1];
    char* outputFile = argv[argc - 1];

    if(strcmp(primitiva, "sphere") == 0)
    {
        float raio = std::stof(argv[2]);
        int slices = std::stoi(argv[3]);
        int stacks = std::stoi(argv[4]);
        geraEsfera(raio, slices, stacks, outputFile);
    }

    return 0;
}