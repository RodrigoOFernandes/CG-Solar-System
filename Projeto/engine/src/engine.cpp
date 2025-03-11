#include "../include/engine.hpp"

Config configuration;

void resize(int w, int h) {
    if (h == 0)
        h = 1;

    float aspect_ratio = static_cast<float>(w) / static_cast<float>(h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(configuration.camera.projection.fov, aspect_ratio, 
                   configuration.camera.projection.near, configuration.camera.projection.far);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}



void renderScene() {
    configuration.draw();
}

int main(int argc, char **argv) {

    if(argc < 2) {
        std::cout << "Please indicate a configuration XML file\n";
    }

    configuration.parseFile(argv[1]);
    configuration.print();
    
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