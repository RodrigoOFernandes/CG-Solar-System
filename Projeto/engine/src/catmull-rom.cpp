#include "../include/catmull-rom.hpp"
#include <glm/glm.hpp>
#include <vector>
#include "stdio.h"
#include <cmath> 

static const std::array<std::array<float, 4>, 4> catmoll_rom_matrix{{
    {-0.5f, +1.5f, -1.5f, +0.5f},
    {+1.0f, -2.5f, +2.0f, -0.5f},
    {-0.5f, +0.0f, +0.5f, +0.0f},
    {+0.0f, +1.0f, +0.0f, +0.0f},
}};

std::array<float, 16> rotationMatrix(glm::vec3 x, glm::vec3 y, glm::vec3 z) {
    return std::array<float, 16>{{
        x.x,
        x.y,
        x.z,
        0,
        y.x,
        y.y,
        y.z,
        0,
        z.x,
        z.y,
        z.z,
        0,
        0,
        0,
        0,
        1,
    }};
  }

std::pair<glm::vec3, glm::vec3> getCatmullRomPosition(std::vector<glm::vec3> control_points, float global_time){
    const std::array<std::array<float, 4>, 4> matrix = catmoll_rom_matrix;
    float t = global_time * control_points.size();
    float segment = (int)floor(t);
    t -= segment;
    int first = segment + control_points.size() - 1;

    glm::vec3 p1 = control_points[(first + 0) % control_points.size()];
    glm::vec3 p2 = control_points[(first + 1) % control_points.size()];
    glm::vec3 p3 = control_points[(first + 2) % control_points.size()];
    glm::vec3 p4 = control_points[(first + 3) % control_points.size()];

    const std::array<std::array<float, 4>, 3> p{{
        {p1.x, p2.x, p3.x, p4.x},
        {p1.y, p2.y, p3.y, p4.y},
        {p1.z, p2.z, p3.z, p4.z},
    }};
  
    const std::array<float, 4> timeP = {t * t * t, t * t, t,
                                        1};  // time matrix for point
    const std::array<float, 4> timeDP = {3 * t * t, 2 * t, 1,
                                         0};  // time matrix for derivate

    std::array<float, 3> pv{};  // Point
    std::array<float, 3> dv{};  // Derivative
    
    for (size_t i = 0; i < 3; ++i) {
        std::array<float, 4> a{};
    
        for (size_t j = 0; j < 4; ++j) {
          for (size_t k = 0; k < 4; ++k) {
            a[j] += p[i][k] * matrix[j][k];
          }
        }
    
        for (size_t j = 0; j < 4; j++) {
          pv[i] += timeP[j] * a[j];
          dv[i] += timeDP[j] * a[j];
        }
      }

    return {
        glm::vec3(pv[0], pv[1], pv[2]),
        glm::vec3(dv[0], dv[1], dv[2]),
    };
    
}

glm::mat4 translationMatrix(float global_time, std::vector<glm::vec3> control_points, bool align){
    std::pair<glm::vec3, glm::vec3> position_dir =
                        getCatmullRomPosition(control_points,global_time);

    glm::vec3 pos = position_dir.first;
    glm::vec3 dir = position_dir.second;
    glm::vec3 y_axis (0,1,0);

    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, glm::vec3(pos.x, pos.y, pos.z));

    if(align){
        glm::vec3 x = glm::normalize(dir);
        glm::vec3 z = glm::normalize(glm::cross(x, y_axis));
        glm::vec3 y = glm::normalize(glm::cross(z, x));
        std::array<float, 16> matrixR = rotationMatrix(x, y, z);
        glm::mat4 rotation_matrix = glm::make_mat4(matrixR.data());
    
        matrix *= rotation_matrix;
    }

    return matrix;
}

