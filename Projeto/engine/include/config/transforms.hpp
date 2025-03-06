#ifndef TRANSFORMS_HPP
#define TRANSFORMS_HPP

#include "../include/parser/tinyxml2.h"
#include <string>
#include <iostream>

#define ROTATE 0
#define TRANSLATE 1
#define SCALE 2

struct Translate {
    int x, y, z;
};

struct Rotate {
    int angle, x, y, z;
};

struct Scale {
    int x, y, z;
};

class Transforms{
    public:
        Translate translate;
        Rotate rotate;
        Scale scale;
        int type;
        
        void parseTransforms(tinyxml2::XMLElement* transformElement);
};

#endif