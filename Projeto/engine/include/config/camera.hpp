#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "../include/parser/tinyxml2.h"
#include <iostream>

struct Vec {
    float x, y, z;
};

struct Projection {
    float fov, near, far;
};


class Camera{
    public:    
        Vec position;
        Vec lookAt;
        Vec up;
        Projection projection;

        void parseCamera(tinyxml2::XMLElement* cameraElement);
        void print() const;
};

#endif
