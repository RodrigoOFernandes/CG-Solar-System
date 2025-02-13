#ifndef MODEL_HPP
#define MODEL_HPP

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define SPHEREMODEL 0
#define CONEMODEL 1
#define PLANEMODEL 2
#define BOXMODEL 3

#include <iostream>
#include <vector>
#include "../include/parser/xmlparser.hpp"


struct Triangle {
    Vec3 v1, v2, v3;
};


class Model{
    public:
        void draw(Config configFile);
        void loadModel(const std::string& filename, int modelFlag); // model flag in #define
        //transformations functions will be included here 
    private:
        std::vector<Triangle> sphere_triangles;
        std::vector<Triangle> box_triangles;
        std::vector<Triangle> plane_triangles;
        std::vector<Triangle> cone_triangles;
};


#endif
