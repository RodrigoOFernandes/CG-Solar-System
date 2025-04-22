#include "../include/engine.hpp"

Config configuration;
char frames[64];
int tbase = 0, t = 0;
float frame = 0.0f, fps = 0.0f;

// ======== Novas variáveis para rato/teclado =========
int lastMouseX = -1, lastMouseY = -1;
bool mouseLeftPressed = false;
bool view_axis = true;
// ============================================

void resize(int w, int h) {
    if (h == 0) h = 1;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    configuration.draw(view_axis);

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

// ======== Callbacks de rato/teclado =========

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftPressed = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (state == GLUT_UP) {
            mouseLeftPressed = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (!mouseLeftPressed) return;

    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    configuration.camera.updateOrbit(deltaX, deltaY);

    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay();
}

void processKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 't':
        case 'T':
            view_axis = !view_axis;
            std::cout << "View axis toggled to: " << (view_axis ? "ON" : "OFF") << std::endl;
            glutPostRedisplay();
            break;
    }
}
// ======================================

int main(int argc, char **argv) {
    if(argc < 2) {
        std::cout << "Please indicate a configuration XML file\n";
        return 1;
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
    glutKeyboardFunc(processKeys);

    // === Registo de callbacks de rato ===
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    // ====================================

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    configuration.parseFile(argv[1]);

    glutReshapeWindow(configuration.window.width, configuration.window.height);
    resize(configuration.window.width, configuration.window.height);

    glutMainLoop();
    return 0;
}
