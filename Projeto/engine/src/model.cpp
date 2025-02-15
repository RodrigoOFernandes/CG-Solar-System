#include "../include/model.hpp"



int Model::loadModel(const std::string& filename) {
    std::string fullPath = "../models/" + filename;
    std::ifstream file(fullPath);
    std::vector<Triangle> triangles; 
    int modelFlag = -1;

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << fullPath << std::endl;
        return -1;
    }

    std::vector<Vec3> vertexes;
    std::string line;

    // first line indicates the model flag of the file
    std::getline(file, line);
    
    if (line == "sphere") modelFlag = SPHEREMODEL;
    else if (line == "cone") modelFlag = CONEMODEL;
    else if (line == "box") modelFlag = BOXMODEL;
    else if (line == "plane") modelFlag = PLANEMODEL;
    else if(line == "cylinder") modelFlag = CYLINDERMODEL;
    else {
        std::cerr << "Unknown Model: " << line << std::endl;
        return -1;
    }

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

    switch (modelFlag) {
        case SPHEREMODEL: sphere_triangles = triangles; break;
        case CONEMODEL: cone_triangles = triangles; break;
        case BOXMODEL: box_triangles = triangles; break;
        case PLANEMODEL: plane_triangles = triangles; break;
        case CYLINDERMODEL: cylinder_triangles = triangles; break;
        default: std::cerr << "Invalid model flag\n" << std::endl;
    }

    return modelFlag;
}

void drawTriangles(std::vector<Triangle> triangles){
    glBegin(GL_TRIANGLES);
        for(const auto& triangle: triangles){
            glVertex3f(triangle.v1.x, triangle.v1.y, triangle.v1.z);
            glVertex3f(triangle.v2.x, triangle.v2.y, triangle.v2.z);
            glVertex3f(triangle.v3.x, triangle.v3.y, triangle.v3.z);
        }
    glEnd();
}

void drawAxis(void){
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
}

void Model::draw (Config configFile){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(configFile.camera.projection.fov, (configFile.window.height/configFile.window.width),
                    configFile.camera.projection.near,configFile.camera.projection.fov);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Use the Camera struct for gluLookAt
    gluLookAt(configFile.camera.position.x, configFile.camera.position.y, configFile.camera.position.z,
              configFile.camera.lookAt.x, configFile.camera.lookAt.y, configFile.camera.lookAt.z,
              configFile.camera.up.x, configFile.camera.up.y, configFile.camera.up.z);
    

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    drawAxis();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    for(const auto& models: configFile.models){
        switch(models.modelFlag){
            case SPHEREMODEL:   drawTriangles(sphere_triangles);break;
            case CONEMODEL:     drawTriangles(cone_triangles);break;
            case PLANEMODEL:    drawTriangles(plane_triangles);break;
            case BOXMODEL:      drawTriangles(box_triangles);break;
            case CYLINDERMODEL: drawTriangles(cylinder_triangles);break;
        }
    }
    
    glutSwapBuffers();
}
