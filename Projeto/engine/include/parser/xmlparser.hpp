#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "../include/parser/tinyxml2.h"
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

struct Window{
    int height;
    int width;
};


typedef struct ConfigFile{
    Window window;
    Camera camera;
    std::vector<Model> models;
}Config;

Camera parseCamera(tinyxml2::XMLElement* cameraElement);
std::vector<Model> parseModels(tinyxml2::XMLElement* modelsElement);
Window parseWindow(tinyxml2::XMLElement* windowElement);

Config parseFile(tinyxml2::XMLDocument doc);

#endif