#ifndef CATMULL_ROM_HPP
#define CATMULL_ROM_HPP

#include "../include/matrix.hpp"
#include "../include/parser/tinyxml2.h"
#include <vector>

typedef struct point{
    float x,y,z;
} Point;

class Catmull{
    public:
        float time;
        bool align;
        std::vector<Point> curvePoints;

        void parseCatmull();
        float* getNextPos();
        float* getRotMatrix();
};

#endif
