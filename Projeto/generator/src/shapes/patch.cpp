#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "../shapes/patch.hpp"

struct ControlPoint {
    float x, y, z;
};

int binomialCoefficient(int n, int i) {
    int res = 1;
    for(int k = 0; k < i; ++k) {
        res *= (n - k);
        res /= (k + 1);
    }

    return res;
}

float bernstein(int n, int i, float y) {
    int bin = binomialCoefficient(n, i);
    return bin * pow(1 - y, n - i) * pow(y, i);
}

void generatePatch(char* patches_file, int tesselation, char* output_file) {    
    std::ifstream file;
    std::ofstream out(output_file);
    file.open(patches_file);

    std::string line;

    int patches_number;
    int points_number;
        
    if(file.is_open()) {
        std::getline(file, line);
        patches_number = std::stoi(line);

        int points_indexes[patches_number][16];
        for(int i = 0; i < patches_number; i++) {
            std::getline(file, line);
            std::istringstream iss(line);
            std::string token;

            for(int j = 0; j < 16; j++) {
                std::getline(iss, token, ',');
                token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
                token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
                points_indexes[i][j] = std::stoi(token);
            }
        }
                
        std::getline(file, line);
        points_number = std::stoi(line);
        
        ControlPoint control_points[points_number];

        for(int i = 0; i < points_number; i++) {
            std::getline(file, line);
            std::istringstream iss(line);
            
            char comma;

            iss >> control_points[i].x >> comma
                >> control_points[i].y >> comma 
                >> control_points[i].z;
        }

        for(int p = 0; p < patches_number; p++){
            for(int iu = 0; iu <= tesselation; iu++ ) {
                float u = (float)iu/tesselation;
                for(int iv = 0; iv <= tesselation; iv++) {
                    float v = (float)iv/tesselation;

                    float x = 0, y = 0, z = 0;
                    for(int i = 0; i < 4; i++) {
                        float Bu = bernstein(3, i, u);
                        for(int j = 0; j < 4; j++) {
                            float Bv = bernstein(3, j ,v);
                            int idx = points_indexes[p][i * 4 + j];
                            ControlPoint cp = control_points[idx];

                            x += Bu * Bv * cp.x;
                            y += Bu * Bv * cp.y;
                            z += Bu * Bv * cp.z;
                        }
                    }
                    out << x << " " << y << " " << z << "\n";
                }
            }
        }
    } else {
        std::cout << "Couldn't open";
    }
}
