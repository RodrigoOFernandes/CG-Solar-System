#include "../include/engine.hpp"

Config configuration;
char frames[64];
int tbase = 0, t = 0;
float frame = 0.0f, fps = 0.0f;

// ======== Novas variáveis para rato/teclado =========
int lastMouseX = -1, lastMouseY = -1;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;
bool show_catmull = true;
bool view_axis = true;
bool lighting = false;
bool viewNormals = false;
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

void processMenuEvents(int option) {
    switch (option) {
        case 1:
            view_axis = !view_axis;
            std::cout << "View axis: " << (view_axis ? "ON" : "OFF") << std::endl;
            break;
        case 2:
            show_catmull = !show_catmull;
            std::cout << "Catmull: " << (show_catmull ? "ON" : "OFF") << std::endl;
            break;
        case 3:
            viewNormals = !viewNormals;
            std::cout << "View Normals: " << (viewNormals ? "ON" : "OFF") << std::endl;
            break;
        case 4:
            lighting = !lighting;
            std::cout << "Lighting: " << (lighting ? "ON" : "OFF") << std::endl;
            break;
    }

    glutPostRedisplay();
}


void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    

    frame++;
    t = glutGet(GLUT_ELAPSED_TIME);
    if (t - tbase > 1000) { 
        fps = frame * 1000.0f / (t - tbase); 
        tbase = t;
        frame = 0;
    }
    sprintf(frames, "CG-SOLAR-System || FPS: %.2f", fps);
    glutSetWindowTitle(frames);

    configuration.draw(view_axis, show_catmull, lighting, viewNormals);
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
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseRightPressed = true;
            lastMouseY = y;  // Só precisamos da coordenada Y para o zoom
        } else if (state == GLUT_UP) {
            mouseRightPressed = false;
        }
    }
}

void mouseMotion(int x, int y) {
    if (mouseLeftPressed) {
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        configuration.camera.updateOrbit(deltaX, deltaY);

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    } else if (mouseRightPressed) {
        int deltaY = y - lastMouseY;
        configuration.camera.updateZoom(deltaY);
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void processKeys(unsigned char key, int x, int y) {
    switch (key) {
        case 't':
        case 'T':
            view_axis = !view_axis;
            std::cout << "View axis toggled to: " << (view_axis ? "ON" : "OFF") << std::endl;
            glutPostRedisplay();
            break;
        case 'c':
        case 'C':
            show_catmull = !show_catmull;
            std::cout << "Catmull toggled to: " << (show_catmull ? "ON" : "OFF") << std::endl;
            glutPostRedisplay();
            break;
        case 'n':
        case 'N':
            viewNormals = !viewNormals;
            std::cout << "Normals view toggled to: " << (viewNormals ? "ON" : "OFF") << std::endl;
            glutPostRedisplay();
            break;
            
    }
}

void processSpecialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            configuration.camera.updateZoom(-5);  // Zoom in
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            configuration.camera.updateZoom(5);   // Zoom out
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

    ilInit();
    iluInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(processKeys);
    glutSpecialFunc(processSpecialKeys);

    // === Registo de callbacks de rato ===
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    // ====================================

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);

    configuration.parseFile(argv[1]);
    glutReshapeWindow(configuration.window.width, configuration.window.height);
    resize(configuration.window.width, configuration.window.height);

    lighting = setupLights(configuration.lights);
    int menu = glutCreateMenu(processMenuEvents);
    glutAddMenuEntry("Toggle View Axis", 1);
    glutAddMenuEntry("Toggle Catmull Curve", 2);
    glutAddMenuEntry("Toggle Normals", 3);
    glutAddMenuEntry("Toggle Lighting", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON); 
    glutMainLoop();

    return 0;
}
