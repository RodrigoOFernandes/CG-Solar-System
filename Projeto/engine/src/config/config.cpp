#include "../include/config/config.hpp"

void Config::parseFile(const char* filename) {
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
    while (groupElement) {
        Group group;
        group.parseGroup(groupElement);
        groups.push_back(group);
        groupElement = groupElement->NextSiblingElement("group");
    }
}

void Config::print() const {
    std::cout << "Configuration:\n";

    // Print window and camera (assuming they have print functions)
    window.print();
    camera.print();

    // Print all groups
    std::cout << "Groups:\n";
    for (const auto& group : groups) {
        group.print();
    }
}