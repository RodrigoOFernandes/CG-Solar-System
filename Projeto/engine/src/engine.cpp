#include "../include/engine.hpp"

Config configuration;
Model model;

void resize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
    model.draw(configuration);
}

int main(int argc, char **argv) {

    if(argc < 2) {
        std::cout << "Please indicate a configuration XML file\n";
    }

    configuration = parseFile(argv[1]);

    for(const auto& modelFile: configuration.models){ // loading .3d information into memory
        model.loadModel(modelFile.file, modelFile.modelFlag);
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(configuration.window.width, configuration.window.height);
    glutCreateWindow("CG-SOLAR-System");
    
    glutDisplayFunc(renderScene);
    glutReshapeFunc(resize);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glutMainLoop();
    return 0;
}