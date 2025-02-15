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
#define CYLINDERMODEL 4

#include <iostream>
#include <fstream>  
#include <sstream> 
#include <vector>
#include "../include/parser/xmlparser.hpp"


struct Triangle {
    Vec3 v1, v2, v3;
};


class Model{
    public:
        void draw(Config configFile);
        int loadModel(const std::string& filename); // model flag in #define || return modelFlag
        //transformations functions will be included here 
    private:
        std::vector<Triangle> sphere_triangles;
        std::vector<Triangle> box_triangles;
        std::vector<Triangle> plane_triangles;
        std::vector<Triangle> cone_triangles;
        std::vector<Triangle> cylinder_triangles;
};


#endif
