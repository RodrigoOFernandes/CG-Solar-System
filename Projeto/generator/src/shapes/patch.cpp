#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "../shapes/patch.hpp"

struct ControlPoint {
    float x, y, z;
};

struct Vertex {
    float x, y, z;
    float nx, ny, nz;
    float tx, ty;
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

void calculateNormal(ControlPoint p1, ControlPoint p2, ControlPoint p3, float& nx, float& ny, float& nz) {
    float ux = p2.x - p1.x;
    float uy = p2.y - p1.y;
    float uz = p2.z - p1.z;
    
    float vx = p3.x - p1.x;
    float vy = p3.y - p1.y;
    float vz = p3.z - p1.z;
    
    nx = uy * vz - uz * vy;
    ny = uz * vx - ux * vz;
    nz = ux * vy - uy * vx;
    
    // Normalize
    float length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length > 0) {
        nx /= length;
        ny /= length;
        nz /= length;
    }
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
        
        // Escrever o número total de triângulos para o arquivo de saída
        int total_triangles = patches_number * 2 * tesselation * tesselation;
        out << total_triangles << "\n";
        
        for(int p = 0; p < patches_number; p++) {
            // Armazenar pontos da grade para cada patch
            std::vector<ControlPoint> grid_points;
            
            // Gerar pontos da superfície para o patch atual
            for(int iu = 0; iu <= tesselation; iu++) {
                float u = (float)iu/tesselation;
                for(int iv = 0; iv <= tesselation; iv++) {
                    float v = (float)iv/tesselation;
                    
                    ControlPoint point = {0, 0, 0};
                    for(int i = 0; i < 4; i++) {
                        float Bu = bernstein(3, i, u);
                        for(int j = 0; j < 4; j++) {
                            float Bv = bernstein(3, j, v);
                            int idx = points_indexes[p][i * 4 + j];
                            ControlPoint cp = control_points[idx];
                            point.x += Bu * Bv * cp.x;
                            point.y += Bu * Bv * cp.y;
                            point.z += Bu * Bv * cp.z;
                        }
                    }
                    grid_points.push_back(point);
                }
            }
            
            for(int iu = 0; iu < tesselation; iu++) {
                for(int iv = 0; iv < tesselation; iv++) {
                    int index00 = iu * (tesselation + 1) + iv;
                    int index01 = iu * (tesselation + 1) + (iv + 1);
                    int index10 = (iu + 1) * (tesselation + 1) + iv;
                    int index11 = (iu + 1) * (tesselation + 1) + (iv + 1);
                    
                    ControlPoint p00 = grid_points[index00];
                    ControlPoint p01 = grid_points[index01];
                    ControlPoint p10 = grid_points[index10]; 
                    ControlPoint p11 = grid_points[index11];
                    
                    // Calcular normais para cada triângulo
                    float nx1, ny1, nz1; // Normal do primeiro triângulo
                    float nx2, ny2, nz2; // Normal do segundo triângulo
                    
                    calculateNormal(p00, p11, p10, nx1, ny1, nz1);
                    calculateNormal(p00, p01, p11, nx2, ny2, nz2);
                    
                    // Coordenadas de textura
                    float u00 = (float)iu / tesselation;
                    float v00 = (float)iv / tesselation;
                    float u01 = (float)iu / tesselation;
                    float v01 = (float)(iv + 1) / tesselation;
                    float u10 = (float)(iu + 1) / tesselation;
                    float v10 = (float)iv / tesselation;
                    float u11 = (float)(iu + 1) / tesselation;
                    float v11 = (float)(iv + 1) / tesselation;
                    
                    // Primeiro triângulo (p00, p11, p10)
                    out << p00.x << " " << p00.y << " " << p00.z << " ";
                    out << nx1 << " " << ny1 << " " << nz1 << " ";
                    out << u00 << " " << v00 << "\n";
                    
                    out << p11.x << " " << p11.y << " " << p11.z << " ";
                    out << nx1 << " " << ny1 << " " << nz1 << " ";
                    out << u11 << " " << v11 << "\n";
                    
                    out << p10.x << " " << p10.y << " " << p10.z << " ";
                    out << nx1 << " " << ny1 << " " << nz1 << " ";
                    out << u10 << " " << v10 << "\n";

                    // Segundo triângulo (p00, p01, p11)
                    out << p00.x << " " << p00.y << " " << p00.z << " ";
                    out << nx2 << " " << ny2 << " " << nz2 << " ";
                    out << u00 << " " << v00 << "\n";
                    
                    out << p01.x << " " << p01.y << " " << p01.z << " ";
                    out << nx2 << " " << ny2 << " " << nz2 << " ";
                    out << u01 << " " << v01 << "\n";
                    
                    out << p11.x << " " << p11.y << " " << p11.z << " ";
                    out << nx2 << " " << ny2 << " " << nz2 << " ";
                    out << u11 << " " << v11 << "\n";
                }
            }
        }
        
        file.close();
        out.close();
    } else {
        std::cout << "Couldn't open file: " << patches_file << std::endl;
    }
}
