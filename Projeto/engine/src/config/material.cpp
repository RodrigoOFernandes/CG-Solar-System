#include "../include/config/material.hpp"
#include <tinyxml2.h>
#include <iostream>

Material parseMaterial(tinyxml2::XMLElement* colorElement) {
    Material m;

    if (!colorElement)
        return m;


    if (colorElement) {
        for (tinyxml2::XMLElement* child = colorElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
            std::string childName = child->Name();
            int ri = 0, gi = 0, bi = 0;
            float value = 0.0f;
            if (childName == "diffuse") {
                child->QueryIntAttribute("R", &ri);
                child->QueryIntAttribute("G", &gi);
                child->QueryIntAttribute("B", &bi);
                m.diffuse = glm::vec4(ri / 255.0f, gi / 255.0f, bi / 255.0f, 1.0f);
            } 
            else if (childName == "ambient") {
                child->QueryIntAttribute("R", &ri);
                child->QueryIntAttribute("G", &gi);
                child->QueryIntAttribute("B", &bi);
                m.ambient = glm::vec4(ri / 255.0f, gi / 255.0f, bi / 255.0f, 1.0f);
            } 
            else if (childName == "specular") {
                child->QueryIntAttribute("R", &ri);
                child->QueryIntAttribute("G", &gi);
                child->QueryIntAttribute("B", &bi);
                m.specular = glm::vec4(ri / 255.0f, gi / 255.0f, bi / 255.0f, 1.0f);
            } 
            else if (childName == "emissive") {
                child->QueryIntAttribute("R", &ri);
                child->QueryIntAttribute("G", &gi);
                child->QueryIntAttribute("B", &bi);
                m.emission = glm::vec4(ri / 255.0f, gi / 255.0f, bi / 255.0f, 1.0f);
            } 
            else if (childName == "shininess") {
                child->QueryFloatAttribute("value", &value);
                m.shininess = value;
            }
        }
    }

    return m;
}
