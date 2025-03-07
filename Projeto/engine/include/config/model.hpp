#ifndef MODEL_HPP
#define MODEL_HPP

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <iostream>
#include <fstream>  
#include <sstream> 
#include <vector>

#include "../include/parser/tinyxml2.h"

struct Vec3 {
    float x, y, z;
};

struct Triangle {
    Vec3 v1, v2, v3;
};


class Model {
    public:
        std::vector<Triangle> triangles;

        void draw() const;
        void parseModel(tinyxml2::XMLElement* modelElement);
};


#endif
