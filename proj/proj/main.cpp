#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <vector>

#include "engine.h"

#include "GL/glew.h"
#include "GL/freeglut.h"



#define CAPTION "Loading World"
#define FPS 60
using namespace engine;

int WindowHandle = 0;
unsigned int FrameCount = 0;

float CameraDistance = 5;



// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 39.0f, beta = 51.0f;
float r = 5.0f;

Scene* scene;
Camera *mainCamera;


/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}
void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		getchar();
		exit(EXIT_FAILURE);
	}
}

void createShaders()
{
	//default shader
	Shader *defaultShader = new CubeShader("VerticeShader.glsl", "FragmentShader.glsl");
	ShaderManager::Instance()->AddShader("defaultShader", defaultShader);

	//simpleCubeShader
	Shader *cubeShader = new CubeShader("CubeVerticeShader.glsl", "CubeFragmentShader.glsl");
	ShaderManager::Instance()->AddShader("cubeShader", cubeShader);

	checkOpenGLError("ERROR: Could not create shaders.");
}
void destroyShaders()
{
	ShaderManager::Instance()->Destroy();
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

void createTextures() {
	Texture *catTexture = new Texture("csample.png");
	TextureManager::Instance()->AddTexture("cat", catTexture);

	Texture *stoneTexture = new Texture("stone.tga");
	TextureManager::Instance()->AddTexture("stone", stoneTexture);
	
	checkOpenGLError("ERROR: Could not create textures.");
}
void destroyTextures()
{
	TextureManager::Instance()->Destroy();
	checkOpenGLError("ERROR: Could not destroy textures.");
}

void createMeshes() {
	Mesh* cubeMesh = new Mesh(std::string("cube.obj"));
	MeshManager::Instance()->AddMesh("cube", cubeMesh);

	
	checkOpenGLError("ERROR: Could not create meshes.");
}
void destroyMeshes() {
	MeshManager::Instance()->Destroy();
}

void createMaterials() {
}
void destroyMaterials() {
	MaterialManager::Instance()->Destroy();
}

void createCameras() {
	mainCamera = new Camera(vec4());
	
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	mainCamera->setViewMatrix(matFactory::LookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
	mainCamera->setProjMatrix(matFactory::PerspectiveProjection(60, (float)WinX / WinY, 0.1f, 20));
}
void destroyCameras() {
	delete(mainCamera);
}

/////////////////////////////////////////////////////////////////////// SCENE
void createScene() {
	scene = new Scene(mainCamera, ShaderManager::Instance()->GetShader("cubeShader"));

	SceneNode *root, *cube;

	root = new SceneNode();
	root->setMatrix(matFactory::Identity4());
    scene->setRoot(root);

	cube = new SceneNode();
    cube->setMesh(MeshManager::Instance()->GetMesh("cube"));
	cube->setTexture(TextureManager::Instance()->GetTexture("stone"));
	cube->setMatrix(matFactory::Identity4());
	cube->setShader(ShaderManager::Instance()->GetShader("cubeShader"));
	root->addNode(cube);

	checkOpenGLError("ERROR: Could not build scene.");

}
void destroyScene() {
	delete(scene);
}

void drawScene()
{
	
	scene->draw();

	checkOpenGLError("ERROR: Could not draw scene.");
}


/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyMeshes();
	destroyTextures();
	destroyShaders();
	destroyCameras();
	destroyScene();	
}

void display()
{
	++FrameCount;
	

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawScene();
	glutSwapBuffers();
}

void idle()
{
	
}

void reshape(int w, int h)
{
	float ratio;
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	mainCamera->setProjMatrix(matFactory::PerspectiveProjection(53.13f, ratio, 0.1f, 20.0f));
}
void update() {}
void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount * 60 << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
	FrameCount = 0;
	glutPostRedisplay();
	glutTimerFunc(1000, timer, 0);
}
void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, refresh, 0);
}

void OnKeydown(unsigned char key, int x, int y) {
	key = tolower(key);
		
	if (key == 27) {
		glutLeaveMainLoop();
	}
	else if (key == 't') {}
}

void OnMouseDown(int button, int state, int xx, int yy) {
	// start tracking the mouse
	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

void OnMouseMove(int xx, int yy) {
	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX = -xx + startX;
	deltaY = yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

	mainCamera->setViewMatrix(matFactory::LookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));

	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}

void mouseWheel(int button, int direction, int x, int y)
{
	r -= direction * 0.7f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	mainCamera->setViewMatrix(matFactory::LookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));


	//  uncomment this if not using an idle func
	//	glutPostRedisplay();
}

	

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(OnKeydown);
	glutMouseFunc(OnMouseDown);
	glutMotionFunc(OnMouseMove);
	glutMouseWheelFunc(mouseWheel);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, refresh, 0);
}

void setupOpenGL() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CLIP_DISTANCE0);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if (WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

	createShaders();
	createMeshes();
	createMaterials();
	createTextures();
	createCameras();
	createScene();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
