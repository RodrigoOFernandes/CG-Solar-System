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

int binomialCoefficient(int n, int i) {
    int res = 1;
    for(int k = 0; k < i; ++k) {
        res *= (n - k);
        res /= (k + 1);
    }
    return res;
}

float bernstein(int n, int i, float t) {
    return binomialCoefficient(n, i) * pow(1 - t, n - i) * pow(t, i);
}

float bernsteinDerivative(int n, int i, float t) {
    return binomialCoefficient(n, i) * (i * pow(t, i - 1) * pow(1 - t, n - i) -
                                        (n - i) * pow(t, i) * pow(1 - t, n - i - 1));
}

ControlPoint cross(const ControlPoint& a, const ControlPoint& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

void normalize(ControlPoint& v) {
    float len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len > 0.00001f) {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
}

void generatePatch(char* patches_file, int tesselation, char* output_file) {
    std::ifstream file(patches_file);
    std::ofstream out(output_file);
    std::string line;

    if (!file.is_open()) {
        std::cout << "Couldn't open file: " << patches_file << std::endl;
        return;
    }

    int patches_number;
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
    int points_number = std::stoi(line);
    ControlPoint control_points[points_number];

    for(int i = 0; i < points_number; i++) {
        std::getline(file, line);
        std::istringstream iss(line);
        char comma;
        iss >> control_points[i].x >> comma >> control_points[i].y >> comma >> control_points[i].z;
    }

    int total_triangles = patches_number * 2 * tesselation * tesselation;
    out << total_triangles << "\n";

    for(int p = 0; p < patches_number; p++) {
        std::vector<ControlPoint> grid_points;
        std::vector<ControlPoint> grid_normals;

        for(int iu = 0; iu <= tesselation; iu++) {
            float u = (float)iu / tesselation;
            for(int iv = 0; iv <= tesselation; iv++) {
                float v = (float)iv / tesselation;
                
                ControlPoint point = {0, 0, 0};
                ControlPoint du = {0, 0, 0};
                ControlPoint dv = {0, 0, 0};

                for(int i = 0; i < 4; i++) {
                    float Bu = bernstein(3, i, u);
                    float dBu = bernsteinDerivative(3, i, u);
                    for(int j = 0; j < 4; j++) {
                        float Bv = bernstein(3, j, v);
                        float dBv = bernsteinDerivative(3, j, v);
                        int idx = points_indexes[p][i * 4 + j];
                        ControlPoint cp = control_points[idx];

                        point.x += Bu * Bv * cp.x;
                        point.y += Bu * Bv * cp.y;
                        point.z += Bu * Bv * cp.z;

                        du.x += dBu * Bv * cp.x;
                        du.y += dBu * Bv * cp.y;
                        du.z += dBu * Bv * cp.z;

                        dv.x += Bu * dBv * cp.x;
                        dv.y += Bu * dBv * cp.y;
                        dv.z += Bu * dBv * cp.z;
                    }
                }

                ControlPoint normal = cross(du, dv);
                normalize(normal);
                grid_points.push_back(point);
                grid_normals.push_back(normal);
            }
        }

        for(int iu = 0; iu < tesselation; iu++) {
            for(int iv = 0; iv < tesselation; iv++) {
                int index00 = iu * (tesselation + 1) + iv;
                int index01 = iu * (tesselation + 1) + (iv + 1);
                int index10 = (iu + 1) * (tesselation + 1) + iv;
                int index11 = (iu + 1) * (tesselation + 1) + (iv + 1);

                auto writeVertex = [&](int index) {
                    ControlPoint p = grid_points[index];
                    ControlPoint n = grid_normals[index];
                    out << p.x << " " << p.y << " " << p.z << " "
                        << n.x << " " << n.y << " " << n.z << " " << "0.0" << " " << "0.0" << "\n";
                };

                writeVertex(index00);
                writeVertex(index11);
                writeVertex(index10);

                writeVertex(index00);
                writeVertex(index01);
                writeVertex(index11);
            }
        }
    }

    file.close();
    out.close();
}

