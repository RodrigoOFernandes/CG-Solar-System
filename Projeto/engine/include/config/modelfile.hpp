#ifndef MODELFILE_HPP
#define MODELFILE_HPP


#include "../include/parser/tinyxml2.h"

#include <string>
#include <vector>
#include <iostream>


struct File {
    std::string filename;
    int modelFlag;
};

class ModelFile{
    public:
        std::vector<File> models;

        void parseModels(tinyxml2::XMLElement* modelsElement);
        void print() const;
};

#endif
