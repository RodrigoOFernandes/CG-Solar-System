#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../include/parser/tinyxml2.h"
#include <iostream>
#include <vector>
#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum LightType { DIRECTIONAL, POINT, SPOT };


typedef struct light{
    LightType type;
    glm::vec4 position;
    glm::vec4 direction;
    float cutoff;
} Light;

void printLights(const std::vector<Light>& lights);
bool setupLights(std::vector<Light> lights);
void drawLights(std::vector<Light> lights);
std::vector<Light> parseLights(tinyxml2::XMLElement* lightsElement);
#endif
