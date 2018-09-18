#include "Lab.h"
#include <time.h> 

#include "Quadric.h"
#include "Disk.h"
#include "CylinderY.h"

//******** Global Variables ***********

Material bronze(color(0.2125, 0.1275, 0.054, 1), color(0.714, 0.4284, 0.18144, 1), color(0.393548, 0.271906, 0.166721, 1), 25.6);
Material gold(color(0.24725,	0.1995,	0.0745,	1), color(0.75164,	0.60648,	0.22648,	1), color(0.628281,	0.555802,	0.366065,	1),51.2); 
Material silver(color(0.19225,	0.19225,	0.19225,	1), color(0.50754,	0.50754,	0.50754,	1), color(0.508273,	0.508273,	0.508273,	1),51.2 );
Material redPlastic(color(0,0,0,1), color(.5,0,0,1), color(.7,.6,.6,1),32 );
int currLight = 0;
bool attenuationIsOn[2] = { false, false };
bool lightIsOn[2] = { false, false };
float FOV = 40;
glm::vec3 attenuationParams[2] = { glm::vec3(1,0,0), glm::vec3(1,0,0) };

void print(const glm::vec3 &v) {
	std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;
}

// Frame buffer for holding pixel color and depth values
FrameBuffer frameBuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

// Ray tracer that generates the rendered image
RayTracer rayTrace(frameBuffer);

// Surfaces or object in the scene to be rendered
std::vector<std::shared_ptr<Surface>> surfaces;

// Light sources in the scene
std::vector<std::shared_ptr<PositionalLight>> lights;

//***********************************
/**
* Acts as the display function for the window.
*/
static void RenderSceneCB() {
	// Get time before rendering the scene
	int frameStartTime = glutGet(GLUT_ELAPSED_TIME);

	// Clear the color buffer
//	frameBuffer.clearColorAndDepthBuffers(); // Not necessary for ray tracing

	// Ray trace the scene to determine the color of all the pixels in the scene
	rayTrace.raytraceScene(surfaces, lights);

	// Display the color buffer
	frameBuffer.showColorBuffer();

	int frameEndTime = glutGet(GLUT_ELAPSED_TIME); // Get end time

	// Calculate and display time to render the scene
	float totalTimeSec = (frameEndTime - frameStartTime) / 1000.0f;
	std::cout << "Render time: " << totalTimeSec << " sec." << std::endl;

} // end RenderSceneCB

// Reset viewport limits for full window rendering each time the window is resized.
// This function is called when the program starts up and each time the window is 
// resized.
static void ResizeCB(int width, int height) {
	// Size the color buffer to match the window size.
	frameBuffer.setFrameBufferSize(width, height);

	rayTrace.setCameraFrame(glm::vec3(0, 0, 15), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	rayTrace.calculatePerspectiveViewingParameters(45.0f);

	// Signal the operating system to re-render the window
	glutPostRedisplay();

} // end ResizeCB

// Create light sources
std::shared_ptr<PositionalLight> light1 = std::make_shared<PositionalLight>(glm::vec3(0, 10, 0));
std::shared_ptr<SpotLight> light2 = std::make_shared<SpotLight>(glm::vec3(0, 10, 0),glm::vec3(glm::vec3(-6,-2,-2)- glm::vec3(0,10,0)));

void buildScene() {
	// Initialize random seed - used to create random colors
	srand((unsigned int)time(NULL));

	// Create objects in the scene
	std::shared_ptr<Plane> plane = std::make_shared <Plane>(glm::vec3(0.0f, -4.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), bronze);

	QuadricParameters sphereParams = { 1.0f, 1.0f, 1.0f, 0, 0, 0, 0, 0, 0, -4 };
	std::shared_ptr<QuadricSurface> sphere = std::make_shared<QuadricSurface>(sphereParams, glm::vec3(-6.0f, -2.0f, -2.0f), gold);
	std::shared_ptr<CylinderY> cylinder = std::make_shared<CylinderY>(glm::vec3(5.0f, -3.0f, 0.0f), 3.0f, 2.0f, silver);
	QuadricParameters hyperParams = { 1.0f, 0.0f, 1.0f, 0, 0, 0, 0, 3, 0, 0 };
	std::shared_ptr<QuadricSurface> paraboloid = std::make_shared<QuadricSurface>(hyperParams, glm::vec3(0.0f, 3.0f, -5.0f), redPlastic);

	surfaces.push_back(plane);
	surfaces.push_back(cylinder);
	surfaces.push_back(sphere);
	surfaces.push_back(paraboloid);

	// Add light sources to vector of light sources
	lights.push_back(light1);
	lights.push_back(light2);
}

void PrintLightInfo() {
	std::cout << "Light #1" << std::endl;
	std::cout << *(lights[0]) << std::endl;
	std::cout << "Light #2" << std::endl;
	std::cout << *((SpotLight*)(lights[1].get())) << std::endl;
}

void incrementClamp(float &v, float delta, float lo, float hi) {
	v = glm::clamp(v + delta, lo, hi);
}

void KeyboardCB(unsigned char key, int x, int y) {
	const float INC = 0.10f;
	SpotLight *sl;
	switch (key) {
	case 'O':
	case 'o':	lights[currLight]->isOn = !lights[currLight]->isOn; break;
	case 'A':
	case 'a':	
		lights[currLight]->atten = !lights[currLight]->atten; 
		attenuationIsOn[currLight] = !attenuationIsOn[currLight];
		break;
	case '1':	currLight = 0; break;
	case '2':	currLight = 1; break;
	case 'w':	sl = (SpotLight*)(lights[1].get());
				incrementClamp(FOV, -5.0f, 0, 70.0f);
				sl->fov = sl->fov - 5;
				if (sl->fov <= 0)
				{
					sl->fov = 0;
				}
				break;
	case 'W':	sl = (SpotLight*)(lights[1].get());
				incrementClamp(FOV, 5.0f, 0, 70.0f);
				sl->fov = sl->fov + 5;
				if (sl->fov >= 360)
				{
					sl->fov = 360;
				}
				break;
	case 'c':	incrementClamp(attenuationParams[currLight].x, -INC, 0.0f, 10.0f);
		lights[currLight]->factor = attenuationParams[currLight];
		break;
	case 'C':	incrementClamp(attenuationParams[currLight].x, INC, 0.0f, 10.0f); lights[currLight]->factor = attenuationParams[currLight]; break;
	case 'l':	incrementClamp(attenuationParams[currLight].y, -INC, 0.0f, 10.0f); lights[currLight]->factor = attenuationParams[currLight]; break;
	case 'L':	incrementClamp(attenuationParams[currLight].y, INC, 0.0f, 10.0f); lights[currLight]->factor = attenuationParams[currLight]; break;
	case 'q':	incrementClamp(attenuationParams[currLight].z, -INC, 0.0f, 10.0f); lights[currLight]->factor = attenuationParams[currLight]; break;
	case 'Q':	incrementClamp(attenuationParams[currLight].z, INC, 0.0f, 10.0f); lights[currLight]->factor = attenuationParams[currLight]; break;
	case 'R':
	case 'r':	attenuationIsOn[currLight]= false;
				attenuationParams[currLight] = glm::vec3(1.0f, 0.0f, 0.0f);
				lights[currLight]->lightPosition = glm::vec3(0.0f, 10.0f, 0.0f);
				break;
	case 27: // Escape key
		glutLeaveMainLoop();
		break;
	default:
		std::cout << key << " key pressed." << std::endl;
	}
	print(attenuationParams[0]);
	print(attenuationParams[1]);
	PrintLightInfo();
	glutPostRedisplay();

} // end KeyboardCB

  // Responds to presses of the arrow keys
static void SpecialKeysCB(int key, int x, int y) {
	const float INC = 0.5f;
	switch (key) {
	case GLUT_KEY_RIGHT: lights[currLight]->lightPosition.x += INC; break;
	case GLUT_KEY_LEFT: lights[currLight]->lightPosition.x -= INC; break;
	case GLUT_KEY_UP: lights[currLight]->lightPosition.y += INC; break;
	case GLUT_KEY_DOWN: lights[currLight]->lightPosition.y -= INC; break;
	case GLUT_KEY_PAGE_UP: lights[currLight]->lightPosition.z += INC; break;
	case GLUT_KEY_PAGE_DOWN: lights[currLight]->lightPosition.z -= INC;  break;
	default:
		std::cout << key << " special key pressed." << std::endl;
	}
	PrintLightInfo();
	glutPostRedisplay();
} // end SpecialKeysCB

int main(int argc, char** argv) {
	// freeGlut and Window initialization ***********************

	// Pass any applicable command line arguments to GLUT. These arguments
	// are platform dependent.
	glutInit(&argc, argv);

	// Set the initial display mode.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA);

	// Set the initial window size
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create a window using a string and make it the current window.
	GLuint world_Window = glutCreateWindow("Ray Trace");

	// Indicate to GLUT that the flow of control should return to the program after
	// the window is closed and the GLUTmain loop is exited.
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Request that the window be made full screen
	//glutFullScreenToggle();

	// Set red, green, blue, and alpha to which the color buffer is cleared.
	frameBuffer.setClearColor(color(0.5f, 0.5f, 1.0f, 1.0f));

	// Callback for window redisplay
	glutDisplayFunc(RenderSceneCB);
	glutReshapeFunc(ResizeCB);
	glutKeyboardFunc(KeyboardCB);
	glutSpecialFunc(SpecialKeysCB);

	buildScene();

	rayTrace.setDefaultColor(color(0.9, 0.9, 0.9, 1));

	// Enter the GLUT main loop. Control will not return until the window is closed.
	glutMainLoop();

	// To keep the console open on shutdown, start the project with Ctrl+F5 instead of just F5.

	return 0;

} // end main