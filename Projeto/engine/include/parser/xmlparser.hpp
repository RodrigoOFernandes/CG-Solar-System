#ifndef XMLPARSER_HPP
#define XMLPARSER_HPP

#include "../include/parser/tinyxml2.h"
#include <string>
#include <vector>
#include <iostream>
#include <string.h>

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

struct ModelFile {
    std::string file;
    int modelFlag; // consult model flags in model.hpp
};

struct Window{
    int64_t height;
    int64_t width;
};


typedef struct ConfigFile{
    Window window;
    Camera camera;
    std::vector<ModelFile> models;
}Config;

Camera parseCamera(tinyxml2::XMLElement* cameraElement);
std::vector<ModelFile> parseModels(tinyxml2::XMLElement* modelsElement);
Window parseWindow(tinyxml2::XMLElement* windowElement);

Config parseFile(char* filename);

#endif