#include "../include/config/camera.hpp"



void Camera::parseCamera(tinyxml2::XMLElement* cameraElement){

    tinyxml2::XMLElement* pos = cameraElement->FirstChildElement("position");
    tinyxml2::XMLElement* lookAt = cameraElement->FirstChildElement("lookAt");
    tinyxml2::XMLElement* up = cameraElement->FirstChildElement("up");
    tinyxml2::XMLElement* proj = cameraElement->FirstChildElement("projection");

    if (pos) pos->QueryFloatAttribute("x", &this->position.x);
    if (pos) pos->QueryFloatAttribute("y", &this->position.y);
    if (pos) pos->QueryFloatAttribute("z", &this->position.z);

    if (lookAt) lookAt->QueryFloatAttribute("x", &this->lookAt.x);
    if (lookAt) lookAt->QueryFloatAttribute("y", &this->lookAt.y);
    if (lookAt) lookAt->QueryFloatAttribute("z", &this->lookAt.z);

    if (up) up->QueryFloatAttribute("x", &this->up.x);
    if (up) up->QueryFloatAttribute("y", &this->up.y);
    if (up) up->QueryFloatAttribute("z", &this->up.z);

    if (proj) proj->QueryFloatAttribute("fov", &this->projection.fov);
    if (proj) proj->QueryFloatAttribute("near", &this->projection.near);
    if (proj) proj->QueryFloatAttribute("far", &this->projection.far);
}