#include "../include/engine.hpp"

Config configuration;
char frames[64];
int tbase = 0, t = 0;
float frame = 0.0f, fps = 0.0f;

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
    frame++;
    t = glutGet(GLUT_ELAPSED_TIME);
    if (t - tbase > 1000) { 
        fps = frame * 1000.0f / (t - tbase); 
        tbase = t;
        frame = 0;
    }

    sprintf(frames, "CG-SOLAR-System || FPS: %.2f", fps);
    glutSetWindowTitle(frames);
    glutPostRedisplay();
    glutSwapBuffers();
}

int main(int argc, char **argv) {

    if(argc < 2) {
        std::cout << "Please indicate a configuration XML file\n";
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);

    glutInitWindowSize(500, 500);
    glutCreateWindow("CG-SOLAR-System");
    
    #if !defined(__APPLE__)
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
            return 1;
        }
    #endif

    glutDisplayFunc(renderScene);
    glutReshapeFunc(resize);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    configuration.parseFile(argv[1]);

    glutReshapeWindow(configuration.window.width, configuration.window.height);
    resize(configuration.window.width, configuration.window.height);
   
    glutMainLoop();
    return 0;
}
