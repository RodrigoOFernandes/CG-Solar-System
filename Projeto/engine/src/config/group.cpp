#include "../include/config/group.hpp"
void Group::parseTransforms(tinyxml2::XMLElement* transformElement) {
    if (!transformElement) {
        std::cerr << "Invalid transform element!" << std::endl;
        return;
    }

    for (tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string childName = child->Name();

        if (childName == "translate") {
            child->QueryFloatAttribute("x", &translate.x);
            child->QueryFloatAttribute("y", &translate.y);
            child->QueryFloatAttribute("z", &translate.z);
            order[0] = 1;
        }
        else if (childName == "rotate") {
            child->QueryFloatAttribute("angle", &rotate.angle);
            child->QueryFloatAttribute("x", &rotate.x);
            child->QueryFloatAttribute("y", &rotate.y);
            child->QueryFloatAttribute("z", &rotate.z);
            order[1] = 2;
        }
        else if (childName == "scale") {
            child->QueryFloatAttribute("x", &scale.x);
            child->QueryFloatAttribute("y", &scale.y);
            child->QueryFloatAttribute("z", &scale.z);
            order[2] = 3;
        }
        else {
            std::cerr << "Unknown transform element: " << childName << std::endl;
        }
    }
}

void Group::parseGroup(tinyxml2::XMLElement* groupElement) {
    if (!groupElement) {
        return;
    }

    tinyxml2::XMLElement* transformElement = groupElement->FirstChildElement("transform");
    if (transformElement) {
        parseTransforms(transformElement);
    }

    tinyxml2::XMLElement* modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        file.parseModels(modelsElement);
    }
}

void Group::print() const {
    std::cout << "Group:\n";

    if (order[0] != 0) {
        std::cout << "  Translate: x=" << translate.x << ", y=" << translate.y << ", z=" << translate.z << "\n";
    }

    if (order[1] != 0) {
        std::cout << "  Rotate: angle=" << rotate.angle << ", x=" << rotate.x << ", y=" << rotate.y << ", z=" << rotate.z << "\n";
    }

    if (order[2] != 0) {
        std::cout << "  Scale: x=" << scale.x << ", y=" << scale.y << ", z=" << scale.z << "\n";
    }

    file.print();
}
