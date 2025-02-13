#include "../include/parser/xmlparser.hpp"

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

std::vector<ModelFile> parseModels(tinyxml2::XMLElement* modelsElement) {
    std::vector<ModelFile> models;
    for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model");
         modelElement;
         modelElement = modelElement->NextSiblingElement("model")) {
        
        ModelFile model;
        const char* file = modelElement->Attribute("file");
        if (file) {
            model.file = file;
            models.push_back(model);
            if(strcmp(file, "sphere.3d") == 0) model.modelFlag = 0;
            if(strcmp(file, "cone.3d") == 0) model.modelFlag = 1;
            if(strcmp(file, "plane.3d") == 0) model.modelFlag = 2;
            if(strcmp(file, "box.3d") == 0){model.modelFlag = 3;} 
            else {model.modelFlag = -1;}
        }
    }
    return models;
}

Window parseWindow(tinyxml2::XMLElement* windowElement){
    Window window;

    if(windowElement) windowElement->QueryInt64Attribute("width", &window.width);
    if(windowElement) windowElement->QueryInt64Attribute("height", &window.height);

    return window;
}

Config parseFile(char* filename){
    Config file;
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load config.xml!" << std::endl;
        return Config();
    }
    
    tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        std::cerr << "No <world> element found!" << std::endl;
        return Config();
    }

    tinyxml2::XMLElement* windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        file.window = parseWindow(windowElement);
    }
    
    
    tinyxml2::XMLElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        file.camera = parseCamera(cameraElement);
    }
    
    tinyxml2::XMLElement* groupElement = worldElement->FirstChildElement("group");
    if (groupElement) {
        tinyxml2::XMLElement* modelsElement = groupElement->FirstChildElement("models");
        if (modelsElement) {
            file.models = parseModels(modelsElement);
        }
    }

    return file;
}