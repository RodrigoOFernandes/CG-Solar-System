#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../include/parser/tinyxml2.h"
#include "../include/config/window.hpp"
#include "../include/config/camera.hpp"
#include "../include/config/modelfile.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <string.h>


class Config{
    public:
        Window window;
        Camera camera;
        ModelFile model;

        void parseFile(char* filename);

};


#endif
