#include "../include/config/group.hpp"

void Group::parseTransforms(tinyxml2::XMLElement* transformElement) {
    if (!transformElement) {
        std::cerr << "Invalid transform element!" << std::endl;
        return;
    }

    int orderIndex = 1;

    for (tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string childName = child->Name();

        if (childName == "translate") {
            child->QueryFloatAttribute("x", &translate.x);
            child->QueryFloatAttribute("y", &translate.y);
            child->QueryFloatAttribute("z", &translate.z);
            order[0] = orderIndex++; 
        }
        else if (childName == "rotate") {
            child->QueryFloatAttribute("angle", &rotate.angle);
            child->QueryFloatAttribute("x", &rotate.x);
            child->QueryFloatAttribute("y", &rotate.y);
            child->QueryFloatAttribute("z", &rotate.z);
            order[1] = orderIndex++;
        }
        else if (childName == "scale") {
            child->QueryFloatAttribute("x", &scale.x);
            child->QueryFloatAttribute("y", &scale.y);
            child->QueryFloatAttribute("z", &scale.z);
            order[2] = orderIndex++;
        }
        else {
            std::cerr << "Unknown transform element: " << childName << std::endl;
        }
    }
}


void Group::parseModels(tinyxml2::XMLElement* modelsElement){
    for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model");
            modelElement;
            modelElement = modelElement->NextSiblingElement("model")){
            
        Model model;
        model.parseModel(modelElement);
        models.push_back(model);
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

    std::vector<std::pair<int, int>> transform_order;

    if (order[0] > 0) transform_order.emplace_back(order[0], TRANSLATE);
    if (order[1] > 0) transform_order.emplace_back(order[1], ROTATE);
    if (order[2] > 0) transform_order.emplace_back(order[2], SCALE);

    std::sort(transform_order.begin(), transform_order.end(),
              [](const std::pair<int, int>& a, const std::pair<int, int>& b) {
                  return a.first < b.first;
              });

    // Aplica transformações na ordem correta
    for (const auto& transform : transform_order) {
        switch (transform.second) {
            case TRANSLATE:
                glTranslatef(translate.x, translate.y, translate.z);
                break;
            case ROTATE:
                glRotatef(rotate.angle, rotate.x, rotate.y, rotate.z);
                break;
            case SCALE:
                glScalef(scale.x, scale.y, scale.z);
                break;
        }
    }

    for (const auto& model : models) {
        model.draw();
    }

    for (auto& subGroup : subGroups) {
        subGroup.drawGroup();
    }

    glPopMatrix();
}



void Group::print(int depth) const {
    std::string indent(depth * 2, ' ');

    std::cout << indent << "Group:\n";

    if (order[0] != 0) {
        std::cout << indent << "  Translate: x=" << translate.x << ", y=" << translate.y << ", z=" << translate.z << "\n";
    }

    if (order[1] != 0) {
        std::cout << indent << "  Rotate: angle=" << rotate.angle << ", x=" << rotate.x << ", y=" << rotate.y << ", z=" << rotate.z << "\n";
    }

    if (order[2] != 0) {
        std::cout << indent << "  Scale: x=" << scale.x << ", y=" << scale.y << ", z=" << scale.z << "\n";
    }

    if (!models.empty()) {
        std::cout << indent << "  Models:\n";
        for (const auto& model : models) {
            std::cout << indent << "    - Model:\n"; 
        }
    }

    if (!subGroups.empty()) {
        std::cout << indent << "  SubGroups:\n";
        for (const auto& subGroup : subGroups) {
            subGroup.print(depth + 1); 
        }
    }
}

