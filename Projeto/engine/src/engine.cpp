#include "../include/engine.hpp"

Config configuration;
Model model;

void resize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
    gluPerspective(configuration.camera.projection.fov, (configuration.window.height/configuration.window.width),
    configuration.camera.projection.near,configuration.camera.projection.far);

	// return to the model view matrix mode
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

    for(auto& modelFile: configuration.models){ // loading .3d information into memory
        modelFile.modelFlag = model.loadModel(modelFile.file);
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