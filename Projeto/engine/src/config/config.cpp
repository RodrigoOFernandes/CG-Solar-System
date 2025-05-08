#include "../include/config/config.hpp"

void Config::parseFile(const char* filename) {
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load config.xml!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        std::cerr << "No <world> element found!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* windowElement = worldElement->FirstChildElement("window");
    if (windowElement) {
        window.parseWindow(windowElement);
    }

    tinyxml2::XMLElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        camera.parseCamera(cameraElement);
    }

    tinyxml2::XMLElement* lightElement = worldElement->FirstChildElement("lights");
    if (lightElement) {
        lights = parseLights(lightElement);
        printLights(lights);
    }

    tinyxml2::XMLElement* groupElement = worldElement->FirstChildElement("group");
    if(groupElement){
        group.parseGroup(groupElement);
    }
}


void drawAxis(bool lighting){
    if(lighting) glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
    if(lighting) glEnable(GL_LIGHTING);
}


void Config::draw(bool view_axis, bool show_catmull, bool lighting, bool viewNormals){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camera.position.x, camera.position.y, camera.position.z,
              camera.lookAt.x, camera.lookAt.y, camera.lookAt.z,
              camera.up.x, camera.up.y, camera.up.z);


    if(lighting) drawLights(lights);

    group.drawGroup(show_catmull, lighting, viewNormals);
    
    if (view_axis) {
        drawAxis(lighting);
    }
}
