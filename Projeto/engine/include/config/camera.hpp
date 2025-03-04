#ifndef CAMERA_HPP
#define CAMERA_HPP


#include "../include/parser/tinyxml2.h"

struct Vec3 {
    float x, y, z;
};

struct Projection {
    float fov, near, far;
};


class Camera{
    public:    
        Vec3 position;
        Vec3 lookAt;
        Vec3 up;
        Projection projection;

        void  parseCamera(tinyxml2::XMLElement* cameraElement);
};

#endif
