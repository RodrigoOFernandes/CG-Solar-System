#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cmath>
#include "../shapes/patch.hpp"

using namespace std;

struct Point {
    double x, y, z;
    Point() : x(0), y(0), z(0) {}
    Point(double x, double y, double z) : x(x), y(y), z(z) {}
    
    Point operator+(const Point& p) const {
        return Point(x + p.x, y + p.y, z + p.z);
    }
    
    Point operator*(double s) const {
        return Point(x * s, y * s, z * s);
    }
};

vector<vector<int>> patches;
vector<Point> controlPoints;

void parse(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;

    getline(file, line);
    int numPatches = stoi(line);
    
    patches.resize(numPatches);

    for(int i = 0; i < numPatches; i++) {
        getline(file, line);
        stringstream ss(line);
        string index;

        while(getline(ss, index, ',')) {
            patches[i].push_back(stoi(index));
        }
    }

    getline(file, line);
    int numControlPoints = stoi(line);
    controlPoints.resize(numControlPoints);

    for(int i = 0; i < numControlPoints; i++) {
        getline(file, line);
        stringstream ss(line);
        string coord;
        vector<double> coords;

        while(getline(ss, coord, ',')) {
            coords.push_back(stod(coord));
        }

        if(coords.size() == 3) {
            controlPoints[i] = Point(coords[0], coords[1], coords[2]);
        }
    }
    
    file.close();
}

Point evaluateBezierCurve(const Point P[4], double t) {
    double t2 = t * t;
    double t3 = t2 * t;
    double mt = 1 - t;
    double mt2 = mt * mt;
    double mt3 = mt2 * mt;
    
    Point result = P[0] * mt3;
    result = result + P[1] * (3 * mt2 * t);
    result = result + P[2] * (3 * mt * t2);
    result = result + P[3] * t3;
    
    return result;
}

Point evaluateBezierPatch(const vector<Point>& patchPoints, double u, double v) {
    Point uCurve[4];
    
    // First evaluate 4 curves along u direction
    for (int i = 0; i < 4; ++i) {
        Point curveP[4];
        for (int j = 0; j < 4; ++j) {
            curveP[j] = patchPoints[i * 4 + j];
        }
        uCurve[i] = evaluateBezierCurve(curveP, u);
    }
    
    // Then evaluate the final point along v direction
    return evaluateBezierCurve(uCurve, v);
}

void generatePatch(string filename, int tessellation, string output) {
    parse(filename);
    
    ofstream outFile(output);
    if (!outFile.is_open()) {
        cerr << "Error creating output file: " << output << endl;
        return;
    }
    
    for (const auto& patchIndices : patches) {
        // Get the 16 control points for this patch
        vector<Point> patchPoints;
        for (int index : patchIndices) {
            patchPoints.push_back(controlPoints[index]);
        }
        
        // Evaluate points on the surface
        vector<Point> patchVertices;
        for (int j = 0; j <= tessellation; ++j) {
            double v = j / (double)tessellation;
            for (int i = 0; i <= tessellation; ++i) {
                double u = i / (double)tessellation;
                patchVertices.push_back(evaluateBezierPatch(patchPoints, u, v));
            }
        }
        
        // Generate triangles with right-hand rule and output vertices directly
        for (int row = 0; row < tessellation; ++row) {
            for (int col = 0; col < tessellation; ++col) {
                int idx = row * (tessellation + 1) + col;
                
                // Get the four vertices of this quad
                Point& v0 = patchVertices[idx];
                Point& v1 = patchVertices[idx + tessellation + 1];
                Point& v2 = patchVertices[idx + tessellation + 2];
                Point& v3 = patchVertices[idx + 1];
            
outFile << v1.x << " " << v1.y << " " << v1.z << "\n";
outFile << v0.x << " " << v0.y << " " << v0.z << "\n";
outFile << v3.x << " " << v3.y << " " << v3.z << "\n";

// Triângulo 2: v1, v3, v2
outFile << v1.x << " " << v1.y << " " << v1.z << "\n";
outFile << v3.x << " " << v3.y << " " << v3.z << "\n";
outFile << v2.x << " " << v2.y << " " << v2.z << "\n";
            }

        }
    }
    
    outFile.close();
    cout << "Triangle vertices written to " << output << endl;
}


