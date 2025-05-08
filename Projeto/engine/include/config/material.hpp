#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "../include/parser/tinyxml2.h"

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct material{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    glm::vec4 emission;
    float shininess;

    material()
        : ambient(0.2f, 0.2f, 0.2f, 1.0f),
          diffuse(0.8f, 0.8f, 0.8f, 1.0f),
          specular(0.0f, 0.0f, 0.0f, 1.0f),
          emission(0.0f, 0.0f, 0.0f, 1.0f),
          shininess(0.0f) {}
} Material;


Material parseMaterial(tinyxml2::XMLElement* modelsElement);
void printMaterial(const Material& mat);
#endif
