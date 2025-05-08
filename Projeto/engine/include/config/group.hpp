#ifndef GROUP_HPP
#define GROUP_HPP


#include "../include/parser/tinyxml2.h"
#include "../include/config/model.hpp"
#include "../include/catmull-rom.hpp"


#include <iostream>
#include <vector>

#define TRANSLATIONS 0
#define ROTATIONS 1
#define STATIC 2


class Group{
    public: 
        glm::mat4 static_transformations = glm::mat4(1.0f);
        glm::mat3 translations = glm::mat3(1.0f);
        glm::mat3 rotations = glm::mat3(1.0f);
        std::vector<glm::vec3> controlPoints;
        bool has_animated_rotation = false;
        glm::vec3 animated_rotation_axis = glm::vec3(0.0f); 
        bool align;
        float time;
        std::vector<int> order;
        mutable float elapsed_time = 0.0f;
        std::vector<Group> subGroups;
        std::vector<Model> models;
        
        Material material;

        void parseGroup(tinyxml2::XMLElement* groupElement);
        void parseTransforms(tinyxml2::XMLElement* transformElement);
        void parseModels(tinyxml2::XMLElement* modelsElement);
        void drawGroup(bool show_catmull, bool lighting, bool viewNormals)  const;
        void applyTransformations(float speed_factor, bool show_catmull) const;
        glm::mat4 applyTimeRotation(float elapsed_time) const;
        glm::mat4 applyTimeTranslation(float elapsed_time, std::vector<glm::vec3> control_points) const;

};

#endif
