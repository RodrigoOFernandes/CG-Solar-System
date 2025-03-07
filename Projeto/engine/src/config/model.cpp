#include "../include/config/model.hpp"


void Model::draw() const{
    glBegin(GL_TRIANGLES); // desenho imediato, futuramente adicionar possibilidade de desenho atraves de VBOs
        for(const auto& triangle: triangles){
            glVertex3f(triangle.v1.x, triangle.v1.y, triangle.v1.z);
            glVertex3f(triangle.v2.x, triangle.v2.y, triangle.v2.z);
            glVertex3f(triangle.v3.x, triangle.v3.y, triangle.v3.z);
        }
    glEnd();
}


void Model::parseModel(tinyxml2::XMLElement* modelElement){
        const char* filename = modelElement->Attribute("file");
        std::string fullPath = std::string("../models/") + std::string(filename);
        std::ifstream file(fullPath);

        std::vector<Vec3> vertexes;
        std::string line;
    
        // first line indicates the model of the file
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            Vec3 vertex;
            if (iss >> vertex.x >> vertex.y >> vertex.z) {
                vertexes.push_back(vertex);
            }
        }
        file.close();

        for (size_t i = 0; i < vertexes.size(); i += 3) {
            if (i + 2 < vertexes.size()) {
                Triangle triangle;
                triangle.v1 = vertexes[i];
                triangle.v2 = vertexes[i + 1];
                triangle.v3 = vertexes[i + 2];
                triangles.push_back(triangle);
            }
        }
}