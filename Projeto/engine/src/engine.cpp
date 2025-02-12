#include "../include/tinyxml2.h"
#include <iostream>

int main() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("config.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load file!" << std::endl;
        return 1;
    }

    // Access the root element ("world" in this case)
    tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        std::cerr << "No <world> element found!" << std::endl;
        return 1;
    }

    // Example: Access an attribute or child element of <world>
    const char* worldName = worldElement->Attribute("window"); // Assuming <world> has a "name" attribute
    if (worldName) {
        printf("Name of world: %s\n", worldName);
    } else {
        std::cerr << "No 'name' attribute found in <world>!" << std::endl;
    }

    return 0;
}