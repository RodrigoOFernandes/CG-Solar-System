#include "../include/config/model.hpp"

void Model::draw() const {
    if (vboID == 0) return;
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, triangle_count);

    glDisableClientState(GL_VERTEX_ARRAY);
}


void Model::parseModel(tinyxml2::XMLElement* modelElement) {
    const char* filename = modelElement->Attribute("file");
    std::string fullPath = std::string("../models/") + std::string(filename);
    std::ifstream file(fullPath);
    std::vector<float> triangles; // Agora diretamente armazenado como floats
    
    if (!file) {
        std::cerr << "Erro ao abrir ficheiro: " << fullPath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Ignora a primeira linha (modelo do ficheiro)

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float x, y, z;
        if (iss >> x >> y >> z) {
            triangles.push_back(x);
            triangles.push_back(y);
            triangles.push_back(z);
        }
    }
    file.close();

    if (triangles.empty()) {
        std::cerr << "Arquivo vazio ou formato inválido: " << fullPath << std::endl;
        return;
    }

    triangle_count = triangles.size() / 3;
    // Criar VBO
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 
                 triangles.size() * sizeof(float), 
                 triangles.data(), 
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "Modelo carregado com sucesso: " << fullPath << ")\n";
}
