#include "../include/config/modelfile.hpp"



void ModelFile::parseModels(tinyxml2::XMLElement* modelsElement){
    for (tinyxml2::XMLElement* modelElement = modelsElement->FirstChildElement("model");
            modelElement;
            modelElement = modelElement->NextSiblingElement("model")) {
   
        File file;
        const char* fileName = modelElement->Attribute("file");
        file.filename = fileName;
        models.push_back(file);
    }
}