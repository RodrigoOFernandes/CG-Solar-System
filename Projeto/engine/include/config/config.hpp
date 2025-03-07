#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../include/parser/tinyxml2.h"
#include "../include/config/window.hpp"
#include "../include/config/group.hpp"
#include "../include/config/camera.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <string.h>


class Config{
    public:
        Window window;
        Camera camera;
        Group group;

        void parseFile(const char* filename);
        void draw();
        void print() const;
};


#endif
