#include "../include/config/group.hpp"

void Group::parseTransforms(tinyxml2::XMLElement* transformElement) {
    if (!transformElement) {
        std::cerr << "Invalid transform element!" << std::endl;
        return;
    }

    // Reinicia a matriz para identidade
    static_transformations = glm::mat4(1.0f);

    for (tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string childName = child->Name();

        if (childName == "translate") {
            float x, y, z;
            child->QueryFloatAttribute("x", &x);
            child->QueryFloatAttribute("y", &y);
            child->QueryFloatAttribute("z", &z);
            static_transformations = glm::translate(static_transformations, glm::vec3(x, y, z));
        }
        else if (childName == "rotate") {
            float angle, x, y, z;
            child->QueryFloatAttribute("angle", &angle);
            child->QueryFloatAttribute("x", &x);
            child->QueryFloatAttribute("y", &y);
            child->QueryFloatAttribute("z", &z);
            static_transformations = glm::rotate(static_transformations, 
                                               glm::radians(angle), 
                                               glm::vec3(x, y, z));
        }
        else if (childName == "scale") {
            float x, y, z;
            child->QueryFloatAttribute("x", &x);
            child->QueryFloatAttribute("y", &y);
            child->QueryFloatAttribute("z", &z);
            static_transformations = glm::scale(static_transformations, glm::vec3(x, y, z));
        }
        else {
            std::cerr << "Unknown transform element: " << childName << std::endl;
        }
    }
}

void Group::parseModels(tinyxml2::XMLElement* modelsElement) {
    for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model");
         modelElement;
         modelElement = modelElement->NextSiblingElement("model")) {
        Model model;
        model.parseModel(modelElement);
        models.push_back(model);
    }
}

void Group::parseGroup(tinyxml2::XMLElement* groupElement) {
    if (!groupElement) return;

    tinyxml2::XMLElement* transformElement = groupElement->FirstChildElement("transform");
    if (transformElement) {
        parseTransforms(transformElement);
    }

    tinyxml2::XMLElement* modelsElement = groupElement->FirstChildElement("models");
    if (modelsElement) {
        parseModels(modelsElement);
    }

    for (tinyxml2::XMLElement* subGroupElement = groupElement->FirstChildElement("group");
         subGroupElement;
         subGroupElement = subGroupElement->NextSiblingElement("group")) {
        Group subGroup;
        subGroup.parseGroup(subGroupElement);
        subGroups.push_back(subGroup);
    }
}

void Group::drawGroup() const {
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(static_transformations)); // applying static transformations

    for (const auto& model : models) {
        model.draw();
    }

    for (const auto& subGroup : subGroups) {
        subGroup.drawGroup();
    }

    glPopMatrix();
}