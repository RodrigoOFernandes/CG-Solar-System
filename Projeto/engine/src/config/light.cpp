#include "../include/config/light.hpp"

std::vector<Light> parseLights(tinyxml2::XMLElement* lightsElement) {
    std::vector<Light> list_lights;

    if (!lightsElement) return list_lights;

    for (tinyxml2::XMLElement* light = lightsElement->FirstChildElement("light");
         light && list_lights.size() < 8;
         light = light->NextSiblingElement("light")) {
        
        const char* type = light->Attribute("type");
        if (!type) continue;

        Light l = {};
        std::string typeStr(type);

        if (typeStr == "point") {
            float x = light->FloatAttribute("posx", 0.0f);
            float y = light->FloatAttribute("posy", 0.0f);
            float z = light->FloatAttribute("posz", 0.0f);

            l.type = POINT;
            l.position = glm::vec4(x, y, z, 1.0f);
            l.direction = glm::vec4(0.0f);
            l.cutoff = 0.0f;
        }
        else if (typeStr == "directional") {
            float dx = light->FloatAttribute("dirx", 0.0f);
            float dy = light->FloatAttribute("diry", 0.0f);
            float dz = light->FloatAttribute("dirz", 0.0f);

            l.type = DIRECTIONAL;
            l.direction = glm::vec4(dx, dy, dz, 0.0f);
            l.position = glm::vec4(0.0f);
            l.cutoff = 0.0f;
        }
        else if (typeStr == "spot") {
            float x = light->FloatAttribute("posx", 0.0f);
            float y = light->FloatAttribute("posy", 0.0f);
            float z = light->FloatAttribute("posz", 0.0f);
            float dx = light->FloatAttribute("dirx", 0.0f);
            float dy = light->FloatAttribute("diry", 0.0f);
            float dz = light->FloatAttribute("dirz", 0.0f);
            float cutoff = light->FloatAttribute("cutoff", 0.0f);

            l.type = SPOT;
            l.position = glm::vec4(x, y, z, 1.0f);
            l.direction = glm::vec4(dx, dy, dz, 0.0f);
            l.cutoff = cutoff;
        }

        list_lights.push_back(l);
    }

    return list_lights;
}

void printLights(const std::vector<Light>& lights) {
    std::cout << "===== LIGHTS =====" << std::endl;
    for (size_t i = 0; i < lights.size(); ++i) {
        const Light& l = lights[i];
        std::cout << "Light " << i << ":" << std::endl;

        switch (l.type) {
            case POINT:
                std::cout << "  Type: Point" << std::endl;
                std::cout << "  Position: (" << l.position.x << ", " << l.position.y << ", " << l.position.z << ")" << std::endl;
                break;

            case DIRECTIONAL:
                std::cout << "  Type: Directional" << std::endl;
                std::cout << "  Direction: (" << l.direction.x << ", " << l.direction.y << ", " << l.direction.z << ")" << std::endl;
                break;

            case SPOT:
                std::cout << "  Type: Spot" << std::endl;
                std::cout << "  Position: (" << l.position.x << ", " << l.position.y << ", " << l.position.z << ")" << std::endl;
                std::cout << "  Direction: (" << l.direction.x << ", " << l.direction.y << ", " << l.direction.z << ")" << std::endl;
                std::cout << "  Cutoff: " << l.cutoff << std::endl;
                break;

            default:
                std::cout << "  Type: Unknown" << std::endl;
        }
    }
}


bool setupLights(std::vector<Light> lights) {
    if (lights.size() != 0) {
      glEnable(GL_RESCALE_NORMAL);
      float amb[4] = {1.0f, 1.0f, 1.0f, 1.0f};
  
      glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
      glEnable(GL_LIGHTING);
      for (int i = 0; i < lights.size(); i++) {
        float white[4] = {1.0, 1.0, 1.0, 1.0};
  
        glEnable(GL_LIGHT0 + i);
        glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, white);
        glLightfv(GL_LIGHT0 + i, GL_SPECULAR, white);
      }
      return true;
    }
  
    return false;
  }
  
  void drawLights(std::vector<Light> lights) {
    for (int i = 0; i < lights.size() && lights.size() < 8; i++) {
      const Light& light = lights[i];
  
      switch (light.type) {
        case DIRECTIONAL: {
          float direction[4] = {light.direction.x, light.direction.y,
                                light.direction.z, 0.0f};
          glLightfv(GL_LIGHT0 + i, GL_POSITION, direction);
          break;
        }
        case POINT: {
          float position[4] = {light.position.x, light.position.y,
                               light.position.z, 1.0f};
          glLightfv(GL_LIGHT0 + i, GL_POSITION, position);
          break;
        }
        case SPOT: {
          float position[4] = {light.position.x, light.position.y,
                              light.position.z, 1.0f};
          glLightfv(GL_LIGHT0 + i, GL_POSITION, position);
          float direction[4] = {light.direction.x, light.direction.y,
                                light.direction.z, 0.0f};
          glLightfv(GL_LIGHT0 + i, GL_SPOT_DIRECTION, direction);
          glLightf(GL_LIGHT0 + i, GL_SPOT_CUTOFF, light.cutoff);
          break;
        }
      }
    }
  }