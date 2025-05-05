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
} Material;


Material parseMaterial(tinyxml2::XMLElement* modelsElement);
void printMaterial(const Material& mat);
#endif
