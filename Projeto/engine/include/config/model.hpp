#ifndef MODEL_HPP
#define MODEL_HPP

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
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
        GLuint vboID;
        Model() : vboID(0) {}
        GLsizei triangle_count;
    
        void parseModel(tinyxml2::XMLElement* modelElement);
        void draw() const;
};


#endif
