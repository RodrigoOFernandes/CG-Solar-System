#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include "../include/parser/tinyxml2.h"

class Window{
    public:
        int64_t height;
        int64_t width;

        void parseWindow(tinyxml2::XMLElement* windowElement);
        void print() const;
};

#endif
