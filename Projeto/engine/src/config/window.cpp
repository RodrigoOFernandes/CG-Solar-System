#include "../include/config/window.hpp"


void Window::parseWindow(tinyxml2::XMLElement* windowElement){
    if(windowElement) windowElement->QueryInt64Attribute("width", &width);
    if(windowElement) windowElement->QueryInt64Attribute("height", &height);
}

void Window::print() const {
    std::cout << "Window: width=" << width << ", height=" << height << "\n";
}