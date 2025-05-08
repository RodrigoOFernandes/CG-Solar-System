#include "../include/config/group.hpp"

void renderCatmullRomCurve(const std::vector<glm::vec3>& control_points) {
    if (control_points.size() < 4) return;
    int samples_per_segment = 100;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < control_points.size() * samples_per_segment; ++i) {
        float global_time = (float)i / (float)(control_points.size() * samples_per_segment);
        glm::vec3 pos = getCatmullRomPosition(control_points, global_time).first;
        glVertex3f(pos.x, pos.y, pos.z);
    }
    glEnd();
}

glm::mat4 ScaleMatrix(float x, float y, float z){
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::scale(matrix, glm::vec3(x, y, z));
    return matrix;
  }
  
  glm::mat4 Translatematrix(float x, float y, float z){
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, glm::vec3(x, y, z));
    return matrix;
  }
  
  
  glm::mat4 Rotationmatrix(float angle, float x, float y, float z){
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::rotate(matrix, glm::radians(angle), glm::vec3(x, y, z));
    return matrix;
  }

void Group::parseTransforms(tinyxml2::XMLElement* transformElement) {
    if (!transformElement) {
        std::cerr << "Invalid transform element!" << std::endl;
        return;
    }


    for (tinyxml2::XMLElement* child = transformElement->FirstChildElement(); child; child = child->NextSiblingElement()) {
        std::string childName = child->Name();

        if (childName == "translate") {
            const char* timeAttr = child->Attribute("time");

            if (timeAttr) {  // animated translate
                align = false;
                child->QueryFloatAttribute("time", &time);

                const char* alignAttr = child->Attribute("align");
                if (alignAttr && std::string(alignAttr) == "true") {
                    align = true;
                }

                for (tinyxml2::XMLElement* point = child->FirstChildElement("point");
                     point != nullptr;
                     point = point->NextSiblingElement("point")) {

                    float px, py, pz;
                    point->QueryFloatAttribute("x", &px);
                    point->QueryFloatAttribute("y", &py);
                    point->QueryFloatAttribute("z", &pz);
                    controlPoints.push_back(glm::vec3(px, py, pz));
                }

                order.push_back(TRANSLATIONS);
            }
            else {  // static translate
                
                float x = 0.0f, y = 0.0f, z = 0.0f;
                child->QueryFloatAttribute("x", &x);
                child->QueryFloatAttribute("y", &y);
                child->QueryFloatAttribute("z", &z);
                static_transformations *= Translatematrix(x,y,z); 
                order.push_back(STATIC);
            }
        }
        else if (childName == "rotate") {
            const char* timeAttr = child->Attribute("time");

            if (timeAttr) {  // animated rotation
                child->QueryFloatAttribute("time", &time);
                float x, y, z;
                child->QueryFloatAttribute("x", &x);
                child->QueryFloatAttribute("y", &y);
                child->QueryFloatAttribute("z", &z);

                animated_rotation_axis = glm::vec3(x, y, z);
                has_animated_rotation = true;

                order.push_back(ROTATIONS);
            }
            else {  // static rotation
                float angle, x, y, z;
                child->QueryFloatAttribute("angle", &angle);
                child->QueryFloatAttribute("x", &x);
                child->QueryFloatAttribute("y", &y);
                child->QueryFloatAttribute("z", &z);

                static_transformations *= Rotationmatrix(angle,x,y,z); 
                order.push_back(STATIC);
            }
        }
        else if (childName == "scale") {
            float x, y, z;
            child->QueryFloatAttribute("x", &x);
            child->QueryFloatAttribute("y", &y);
            child->QueryFloatAttribute("z", &z);

            static_transformations *= ScaleMatrix(x,y,z); 
            order.push_back(STATIC);
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

        tinyxml2::XMLElement* colorElement = modelElement->FirstChildElement("color");
        if (colorElement) {
            Material mat = parseMaterial(colorElement);
            model.setMaterial(mat);
            model.printMaterial(); 
        }

        tinyxml2::XMLElement* textureElement = modelElement->FirstChildElement("texture");
        if (textureElement) {
            model.parseTexture(textureElement);
        }

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

glm::mat4 Group::applyTimeRotation(float elapsed_time) const{
    if (this->time == 0) {
      return glm::mat4(1.0f);
    }
    float angle = 360 * (elapsed_time / this->time);
    return glm::rotate(glm::mat4(1.0f), glm::radians(angle), animated_rotation_axis);
}

glm::mat4 Group::applyTimeTranslation(float elapsed_time, std::vector<glm::vec3> control_points) const{
    if (this->time == 0) {
        return glm::mat4(1.0f);
    }

    float global_time = elapsed_time / this->time;

    return translationMatrix(global_time,control_points,align);

}

void Group::applyTransformations(float speed_factor, bool show_catmull) const {
    float elapsed_time = speed_factor * glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    glm::mat4 matrix = glm::mat4(1.0f);
    bool static_applied = false;

    for (int type : order) {
        switch (type) {
          case TRANSLATIONS:
            if(show_catmull){
                renderCatmullRomCurve(controlPoints);
            }
            matrix *= applyTimeTranslation(elapsed_time, controlPoints);
            break;
          case ROTATIONS:
            matrix *= applyTimeRotation(elapsed_time);
            break;
          case STATIC:
            if(static_applied) break; // static transformations are already applied on parsing phase, so we only need to make 1 multiplication
            matrix *= static_transformations;
            static_applied = true;
            break;
        }
        if(static_applied) 
                    continue;
    }
    glMultMatrixf(&matrix[0][0]);
}

void Group::drawGroup(bool show_catmull, bool lights, bool viewNormals) const {
    float speed_factor = time / 10.0f;
    glPushMatrix();

    applyTransformations(speed_factor, show_catmull);
    
    for (const auto& model : models) {
        model.draw(viewNormals);
    }

    for (const auto& subGroup : subGroups) {
        subGroup.drawGroup(show_catmull, lights, viewNormals);
    }

    glPopMatrix();
}