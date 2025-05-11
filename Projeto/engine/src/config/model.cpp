#include "../include/config/model.hpp"

void Model::draw(bool viewNormals) const {
    setupMaterial();
    
    glBindTexture(GL_TEXTURE_2D, textID);
    
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBOId);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, textureVBOId);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, triangle_count);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, 0);

    drawNormals(viewNormals);
}

void Model::loadTexture(std::string filename){
    std::string fullPath = "../textures/" + filename;

    ILuint t;
    ilGenImages(1, &t);
    ilBindImage(t);
    
    if (!ilLoadImage(fullPath.c_str())) {
        std::cerr << "======Erro ao carregar a textura: ====" << fullPath << std::endl;
        ilDeleteImages(1, &t);
        textID = 0;
        exit(EXIT_FAILURE);
        return;
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    int tw = ilGetInteger(IL_IMAGE_WIDTH);
    int th = ilGetInteger(IL_IMAGE_HEIGHT);
    unsigned char* textData = ilGetData();
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textID);
    glBindTexture(GL_TEXTURE_2D, textID);
  
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
  
    // Upload data to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, textData);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glGenerateMipmap(GL_TEXTURE_2D);

    ilDeleteImages(1, &t); 

    glBindTexture(GL_TEXTURE_2D, 0);
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
    std::getline(file, line);

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

    verticesCPU = triangles;
    normalsCPU = normals;
    triangle_count = triangles.size() / 3;

    // Criar VBO para vértices
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, 
                 triangle_count * sizeof(float) * 3, 
                 triangles.data(), 
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &normalVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBOId);
    glBufferData(GL_ARRAY_BUFFER, 
                triangle_count * sizeof(float) * 3, 
                 normals.data(), 
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &textureVBOId);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBOId);
    glBufferData(GL_ARRAY_BUFFER, 
                triangle_count * sizeof(float) * 2, 
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


void Model::drawNormals(bool viewNormals) const {
    if (!viewNormals) return;

    glPushAttrib(GL_ENABLE_BIT); // guarda estado de iluminação
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 0.0f, 0.0f); 
    glBegin(GL_LINES);
    for (size_t i = 0; i < verticesCPU.size(); i += 3) {
        float vx = verticesCPU[i];
        float vy = verticesCPU[i + 1];
        float vz = verticesCPU[i + 2];

        float nx = normalsCPU[i];
        float ny = normalsCPU[i + 1];
        float nz = normalsCPU[i + 2];

        glVertex3f(vx, vy, vz);
        glVertex3f(vx + nx * 0.2f, vy + ny * 0.2f, vz + nz * 0.2f);
    }
    glEnd();

    glPopAttrib(); // restaura estado de iluminação
}

