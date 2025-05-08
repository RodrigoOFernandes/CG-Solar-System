#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../include/parser/tinyxml2.h"
#include "../include/config/window.hpp"
#include "../include/config/group.hpp"
#include "../include/config/camera.hpp"
#include "../include/config/light.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <string.h>


class Config{
    public:
        Window window;
        Camera camera;
        Group group;
        std::vector<Light> lights;

        void parseFile(const char* filename);
        void draw(bool view_axis, bool show_catmull, bool lighting, bool viewNormals);
        void print() const;
};


#endif
