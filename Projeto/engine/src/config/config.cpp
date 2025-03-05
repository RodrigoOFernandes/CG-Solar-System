#include "../include/config/config.hpp"

void Config::parseGroup(tinyxml2::XMLElement* groupElement, const Transforms& parentTransforms) {
    if (!groupElement) {
        return;
    }

    Transforms currentTransforms = parentTransforms;

    tinyxml2::XMLElement* transformElement = groupElement->FirstChildElement("transform");
    if (transformElement) {
        currentTransforms.parseTransforms(transformElement);
    }

    tinyxml2::XMLElement* modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        model.parseModels(modelsElement);
    }

    tinyxml2::XMLElement* childGroupElement = groupElement->FirstChildElement("group");
    while (childGroupElement) {
        parseGroup(childGroupElement, currentTransforms); 
        childGroupElement = childGroupElement->NextSiblingElement("group");
    }
}

void Config::parseFile(char* filename) {
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load config.xml!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        std::cerr << "No <world> element found!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        window.parseWindow(windowElement);
    }

    tinyxml2::XMLElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        camera.parseCamera(cameraElement);
    }

    tinyxml2::XMLElement* groupElement = worldElement->FirstChildElement("group");
    if (groupElement) {
        Transforms initialTransforms; 
        parseGroup(groupElement, initialTransforms);
    }
}