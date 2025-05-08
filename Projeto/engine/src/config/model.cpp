#include "../include/config/model.hpp"

void Model::draw() const {
    // Bind da textura
    if (textID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textID);
    }
    

    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if (normalVBOId != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, normalVBOId);
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, 0);
    }

    if (textureVBOId != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, textureVBOId);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    glDrawArrays(GL_TRIANGLES, 0, triangle_count);

    glDisableClientState(GL_VERTEX_ARRAY);
    if (normalVBOId != 0) {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
    if (textureVBOId != 0) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if(textID != 0){
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}


void Model::loadTexture(std::string filename){
    std::string fullPath = "../textures/" + filename;

    ILuint t;
    ilGenImages(1, &t);
    ilBindImage(t);
    
    if (!ilLoadImage(fullPath.c_str())) {
        std::cerr << "======Erro ao carregar a textura: ====" << fullPath << std::endl;
        ilDeleteImages(1, &t); // limpa mesmo em falha
        textID = 0; // <--- previne o uso indevido
        exit(EXIT_FAILURE);
        return;
    }

    ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE); 
    int tw = ilGetInteger(IL_IMAGE_WIDTH);
    int th = ilGetInteger(IL_IMAGE_HEIGHT);
    unsigned char* textData = ilGetData();
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &textID);
    glBindTexture(GL_TEXTURE_2D, textID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_BGRA, GL_UNSIGNED_BYTE, textData);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glGenerateMipmap(GL_TEXTURE_2D);

    ilDeleteImages(1, &t); 
    glDisable(GL_TEXTURE_2D);
}



void Model::setupMaterial() const{
    glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(material.ambient));
    glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(material.diffuse));
    glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(material.specular));
    glMaterialfv(GL_FRONT, GL_EMISSION, glm::value_ptr(material.emission));
    glMaterialf(GL_FRONT, GL_SHININESS, material.shininess);
}
void Model::parseTexture(tinyxml2::XMLElement* textureElement) {
    if (!textureElement) return;

    const char* file = textureElement->Attribute("file");
    if (file) {
        textureFileName = file;
    } else {
        std::cerr << "Atributo 'file' em <texture> está ausente ou inválido.\n";
    }
    loadTexture(textureFileName);
}

void Model::parseModel(tinyxml2::XMLElement* modelElement) {
    const char* filename = modelElement->Attribute("file");
    std::string fullPath = std::string("../models/") + std::string(filename);
    std::ifstream file(fullPath);
    std::vector<float> triangles;
    std::vector<float> normals;
    std::vector<float> textures;
    
    if (!file) {
        std::cerr << "Erro ao abrir ficheiro: " << fullPath << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // Ignora a primeira linha (modelo do ficheiro)

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        
        float x, y, z, nx, ny, nz, tx, ty;
       
        if (iss >> x >> y >> z >> nx >> ny >> nz >> tx >> ty) {
            triangles.push_back(x);
            triangles.push_back(y);
            triangles.push_back(z);
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
            textures.push_back(tx);
            textures.push_back(ty);
        } else {
            std::cerr << "[ERROR]3d file: Invalid File format"<< "\n"<< "---> Supported .3d file format: 'x y z nx ny nz tx ty'" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    file.close();

    if (triangles.empty()) {
        std::cerr << "Arquivo vazio ou formato inválido: " << fullPath << std::endl;
        return;
    }

    triangle_count = triangles.size() / 3;

    // Criar VBO para vértices
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 
                 triangles.size() * sizeof(float), 
                 triangles.data(), 
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &normalVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBOId);
    glBufferData(GL_ARRAY_BUFFER, 
                 normals.size() * sizeof(float), 
                 normals.data(), 
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBOId);
    glBufferData(GL_ARRAY_BUFFER, 
                 textures.size() * sizeof(float), 
                 textures.data(), 
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "Modelo carregado com sucesso: " << fullPath << ")\n";
}


void Model::setMaterial(const Material& mat) {
    this->material = mat;
}

void Model::parseModelMaterial(tinyxml2::XMLElement* materialElement){
    material = parseMaterial(materialElement);
}

void Model::printMaterial() {
    std::cout << "Material:" << std::endl;
    std::cout << "  Ambient:  (" << material.ambient.r << ", " << material.ambient.g << ", " << material.ambient.b << ", " << material.ambient.a << ")" << std::endl;
    std::cout << "  Diffuse:  (" << material.diffuse.r << ", " << material.diffuse.g << ", " << material.diffuse.b << ", " << material.diffuse.a << ")" << std::endl;
    std::cout << "  Specular: (" << material.specular.r << ", " << material.specular.g << ", " << material.specular.b << ", " << material.specular.a << ")" << std::endl;
    std::cout << "  Emission: (" << material.emission.r << ", " << material.emission.g << ", " << material.emission.b << ", " << material.emission.a << ")" << std::endl;
    std::cout << "  Shininess: " << material.shininess << std::endl;
}
