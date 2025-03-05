#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../include/parser/tinyxml2.h"
#include "../include/config/window.hpp"
#include "../include/config/camera.hpp"
#include "../include/config/modelfile.hpp"
#include "../include/config/transforms.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <string.h>


class Config{
    public:
        Window window;
        Camera camera;
        ModelFile model;
        Transforms transforms;

        void parseGroup(tinyxml2::XMLElement* groupElement, const Transforms& parentTransforms);
        void parseFile(char* filename);
};


#endif
