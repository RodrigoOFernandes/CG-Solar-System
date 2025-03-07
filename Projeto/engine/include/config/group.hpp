#ifndef GROUP_HPP
#define GROUP_HPP


#include "../include/parser/tinyxml2.h"
#include "../include/config/model.hpp"

#include <iostream>
#include <functional>
#include <vector>
#include <algorithm> // Para std::sort


#define TRANSLATE 0
#define ROTATE 1
#define SCALE 2 

struct Translate {
    float x = 0, y = 0, z = 0;
};

struct Rotate {
    float angle = 0, x = 0, y = 0, z = 0;
};

struct Scale {
    float x = 1, y = 1, z = 1;
};

class Group{
    public: 
        Translate translate;
        Rotate rotate;
        Scale scale;
        int order[3] = {0, 0, 0}; //idx 0 -> translate; idx 1 -> rotate; idx 2 -> scale. Valor 0 representa que nao é necessaria aquela transformaçao; 1,2 e 3 representam a order
        std::vector<Group> subGroups;
        std::vector<Model> models;

        void parseGroup(tinyxml2::XMLElement* groupElement);
        void parseTransforms(tinyxml2::XMLElement* transformElement);
        void parseModels(tinyxml2::XMLElement* modelsElement);
        void drawGroup() const;
        void print(int depth) const;

};

#endif
