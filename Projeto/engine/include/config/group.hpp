#ifndef GROUP_HPP
#define GROUP_HPP


#include "../include/parser/tinyxml2.h"
#include "../include/config/modelfile.hpp"
#include "../include/config/transforms.hpp"

#include <iostream>
#include <vector>



class Group{
    public: 
        ModelFile file;
        std::vector<Transforms> transforms;
};


#endif
