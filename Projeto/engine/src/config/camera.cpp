#include "../include/config/camera.hpp"

void Camera::parseCamera(tinyxml2::XMLElement* cameraElement){
    tinyxml2::XMLElement* pos = cameraElement->FirstChildElement("position");
    tinyxml2::XMLElement* look = cameraElement->FirstChildElement("lookAt");
    tinyxml2::XMLElement* u = cameraElement->FirstChildElement("up");
    tinyxml2::XMLElement* proj = cameraElement->FirstChildElement("projection");

    if (pos) { pos->QueryFloatAttribute("x", &position.x); pos->QueryFloatAttribute("y", &position.y); pos->QueryFloatAttribute("z", &position.z); }
    if (look) { look->QueryFloatAttribute("x", &lookAt.x); look->QueryFloatAttribute("y", &lookAt.y); look->QueryFloatAttribute("z", &lookAt.z); }
    if (u) { u->QueryFloatAttribute("x", &up.x); u->QueryFloatAttribute("y", &up.y); u->QueryFloatAttribute("z", &up.z); }
    if (proj) { proj->QueryFloatAttribute("fov", &projection.fov); proj->QueryFloatAttribute("near", &projection.near); proj->QueryFloatAttribute("far", &projection.far); }

    // Cálculo dos ângulos esféricos e raio
    float dx = position.x - lookAt.x;
    float dy = position.y - lookAt.y;
    float dz = position.z - lookAt.z;
    radius = std::sqrt(dx*dx + dy*dy + dz*dz);
    angleX = std::atan2(dz, dx);
    angleY = std::asin(dy / radius);
}


void Camera::updateOrbit(float deltaX, float deltaY) {
    const float sensitivity = 0.005f;
    angleX += deltaX * sensitivity;
    angleY += deltaY * sensitivity;

    // Limita o ângulo vertical
    const float limit = M_PI / 2.0f - 0.01f; // ~89.4°
    if (angleY > limit) angleY = limit;
    if (angleY < -limit) angleY = -limit;

    // Atualiza a posição com base nos ângulos
    position.x = lookAt.x + radius * std::cos(angleY) * std::cos(angleX);
    position.y = lookAt.y + radius * std::sin(angleY);
    position.z = lookAt.z + radius * std::cos(angleY) * std::sin(angleX);
}

void Camera::updateZoom(float deltaY) {
    const float sensitivity = 0.1f;
    radius -= deltaY * sensitivity;
    
    // Limita o raio mínimo e máximo (opcional)
    if (radius < 1.0f) radius = 1.0f;
    if (radius > 100.0f) radius = 100.0f;
    
    // Atualiza a posição com base no novo raio
    position.x = lookAt.x + radius * std::cos(angleY) * std::cos(angleX);
    position.y = lookAt.y + radius * std::sin(angleY);
    position.z = lookAt.z + radius * std::cos(angleY) * std::sin(angleX);
}


