#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "../include/tinyxml2.h"
#include <string>
#include <vector>

struct Vec3 {
    float x, y, z;
};

struct Projection {
    float fov, near, far;
};

struct Camera {
    Vec3 position;
    Vec3 lookAt;
    Vec3 up;
    Projection projection;
};

struct Model {
    std::string file;
};

Camera parseCamera(tinyxml2::XMLElement* cameraElement);
std::vector<Model> parseModels(tinyxml2::XMLElement* modelsElement);

#endif