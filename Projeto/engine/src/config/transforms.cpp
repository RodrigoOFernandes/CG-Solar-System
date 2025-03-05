#include "../include/config/transforms.hpp"

void Transforms::parseTransforms(tinyxml2::XMLElement* transformElement) {
    if (!transformElement) {
        std::cerr << "Invalid transform element!" << std::endl;
        return;
    }

    for (tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string childName = child->Name();

        if (childName == "translate") {
            translate.x = child->IntAttribute("x");
            translate.y = child->IntAttribute("y");
            translate.z = child->IntAttribute("z");
        }
        else if (childName == "rotate") {
            rotate.angle = child->IntAttribute("angle");
            rotate.x = child->IntAttribute("x");
            rotate.y = child->IntAttribute("y");
            rotate.z = child->IntAttribute("z");
        }
        else if (childName == "scale") {
            scale.x = child->IntAttribute("x");
            scale.y = child->IntAttribute("y");
            scale.z = child->IntAttribute("z");
        }
        else {
            std::cerr << "Unknown transform element: " << childName << std::endl;
        }
    }
}