#include "../include/tinyxml2.h"
#include <iostream>
#include <vector>
#include <string>

struct Vec3 {
    float x, y, z;
};

struct Projection {
    float fov, near, far;
};

struct Camera {
    Vec3 position;
    Vec3 lookAt;
    Vec3 up;
    Projection projection;
};

struct Model {
    std::string file;
};

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
