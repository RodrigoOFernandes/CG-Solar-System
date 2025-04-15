#ifndef GROUP_HPP
#define GROUP_HPP


#include "../include/parser/tinyxml2.h"
#include "../include/config/model.hpp"
#include "../include/catmull-rom.hpp"


#include <iostream>
#include <vector>


class Group{
    public: 
        glm::mat4 static_transformations = glm::mat4(1.0f);
        std::vector<Group> subGroups;
        std::vector<Model> models;

        void parseGroup(tinyxml2::XMLElement* groupElement);
        void parseTransforms(tinyxml2::XMLElement* transformElement);
        void parseModels(tinyxml2::XMLElement* modelsElement);
        void drawGroup() const;
        void print(int depth) const;

};

#endif
