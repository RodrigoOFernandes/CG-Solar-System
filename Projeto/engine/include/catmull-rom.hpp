#ifndef CATMULL_ROM_HPP
#define CATMULL_ROM_HPP

#include "../include/parser/tinyxml2.h"
#include <vector>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 translationMatrix(float global_time, std::vector<glm::vec3> control_points, bool align);
std::pair<glm::vec3, glm::vec3> getCatmullRomPosition(std::vector<glm::vec3> control_points, float global_time);
#endif
