#include "../include/xmlparser.hpp"
#include <iostream>

int main() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("config.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load config.xml!" << std::endl;
        return 1;
    }

    tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        std::cerr << "No <world> element found!" << std::endl;
        return 1;
    }

    // Parse Camera
    tinyxml2::XMLElement* cameraElement = worldElement->FirstChildElement("camera");
    Camera camera;
    if (cameraElement) {
        camera = parseCamera(cameraElement);
    }

    // Parse Models
    std::vector<Model> models;
    tinyxml2::XMLElement* groupElement = worldElement->FirstChildElement("group");
    if (groupElement) {
        tinyxml2::XMLElement* modelsElement = groupElement->FirstChildElement("models");
        if (modelsElement) {
            models = parseModels(modelsElement);
        }
    }

    // Print Camera Data
    std::cout << "Camera:" << std::endl;
    std::cout << "  Position: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")" << std::endl;
    std::cout << "  LookAt: (" << camera.lookAt.x << ", " << camera.lookAt.y << ", " << camera.lookAt.z << ")" << std::endl;
    std::cout << "  Up: (" << camera.up.x << ", " << camera.up.y << ", " << camera.up.z << ")" << std::endl;
    std::cout << "  Projection: FOV=" << camera.projection.fov << " Near=" << camera.projection.near << " Far=" << camera.projection.far << std::endl;

    // Print Model Data
    std::cout << "\nModels:" << std::endl;
    for (const auto& model : models) {
        std::cout << "  Model file: " << model.file << std::endl;
    }

    return 0;
}
