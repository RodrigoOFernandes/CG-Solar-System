#include "../include/xmlparser.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include <iostream>

struct Triangle {
    Vec3 v1, v2, v3;
};

std::vector<Triangle> sphereTriangles;

void loadModel(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }
    
    int numVertices;
    file >> numVertices;
    
    std::vector<Vec3> vertices;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Vec3 vertex;
        if (iss >> vertex.x >> vertex.y >> vertex.z) {
            vertices.push_back(vertex);
        }
    }
    file.close();

    for (size_t i = 0; i < vertices.size(); i += 3) {
        if (i + 2 < vertices.size()) {
            Triangle triangle;
            triangle.v1 = vertices[i];
            triangle.v2 = vertices[i + 1];
            triangle.v3 = vertices[i + 2];
            sphereTriangles.push_back(triangle);
        }
    }
}

void glutSphereInitiator(Camera camera, std::vector<Model> models) {
    for (const auto& model : models) {
        loadModel(model.file);
    }
}

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.5f, 1.0f);
    for (const auto& triangle : sphereTriangles) {
        glVertex3f(triangle.v1.x, triangle.v1.y, triangle.v1.z);
        glVertex3f(triangle.v2.x, triangle.v2.y, triangle.v2.z);
        glVertex3f(triangle.v3.x, triangle.v3.y, triangle.v3.z);
    }
    glEnd();
    
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile("config.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load config.xml!" << std::endl;
        return 1;
    }
    
    tinyxml2::XMLElement* worldElement = doc.FirstChildElement("world");
    if (!worldElement) {
        std::cerr << "No <world> element found!" << std::endl;
        return 1;
    }
    
    Camera camera;
    tinyxml2::XMLElement* cameraElement = worldElement->FirstChildElement("camera");
    if (cameraElement) {
        camera = parseCamera(cameraElement);
    }
    
    std::vector<Model> models;
    tinyxml2::XMLElement* groupElement = worldElement->FirstChildElement("group");
    if (groupElement) {
        tinyxml2::XMLElement* modelsElement = groupElement->FirstChildElement("models");
        if (modelsElement) {
            models = parseModels(modelsElement);
        }
    }
    
    glutSphereInitiator(camera, models);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CG@DI-UM Sphere Renderer");
    
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glutMainLoop();
    return 0;
}