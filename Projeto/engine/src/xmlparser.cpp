#include "../include/xmlparser.hpp"

Camera parseCamera(tinyxml2::XMLElement* cameraElement) {
    Camera cam;
    
    tinyxml2::XMLElement* pos = cameraElement->FirstChildElement("position");
    tinyxml2::XMLElement* lookAt = cameraElement->FirstChildElement("lookAt");
    tinyxml2::XMLElement* up = cameraElement->FirstChildElement("up");
    tinyxml2::XMLElement* proj = cameraElement->FirstChildElement("projection");

    if (pos) pos->QueryFloatAttribute("x", &cam.position.x);
    if (pos) pos->QueryFloatAttribute("y", &cam.position.y);
    if (pos) pos->QueryFloatAttribute("z", &cam.position.z);

    if (lookAt) lookAt->QueryFloatAttribute("x", &cam.lookAt.x);
    if (lookAt) lookAt->QueryFloatAttribute("y", &cam.lookAt.y);
    if (lookAt) lookAt->QueryFloatAttribute("z", &cam.lookAt.z);

    if (up) up->QueryFloatAttribute("x", &cam.up.x);
    if (up) up->QueryFloatAttribute("y", &cam.up.y);
    if (up) up->QueryFloatAttribute("z", &cam.up.z);

    if (proj) proj->QueryFloatAttribute("fov", &cam.projection.fov);
    if (proj) proj->QueryFloatAttribute("near", &cam.projection.near);
    if (proj) proj->QueryFloatAttribute("far", &cam.projection.far);

    return cam;
}

std::vector<Model> parseModels(tinyxml2::XMLElement* modelsElement) {
    std::vector<Model> models;
    for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model");
         modelElement;
         modelElement = modelElement->NextSiblingElement("model")) {
        
        Model model;
        const char* file = modelElement->Attribute("file");
        if (file) {
            model.file = file;
            models.push_back(model);
        }
    }
    return models;
}