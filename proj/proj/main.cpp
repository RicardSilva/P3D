///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2016 by Jo�o Madeiras Pereira
// TEMPLATE: Whitted Ray Tracing NFF scenes and drawing points with Modern OpenGL
//
//You should develop your rayTracing( Ray Ray, int depth, float RefrIndex) which returns a color and
// to develop your load_NFF function
//
///////////////////////////////////////////////////////////////////////

//refrationDirection sa
//https://www.gamedev.net/topic/486265-raytracing---refraction-algorithm/

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "scene.h"
#include "vec.h"
#include "Ray.h"

#define CAPTION "Ray tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1
#define ANTI_ALISING_NUMBER 5

#define MAX_DEPTH 3

// Points defined by 2 attributes: positions which are stored in vertices array and colors which are stored in colors array
float *colors;
float *vertices;

int size_vertices;
int size_colors;

GLfloat m[16];  //projection matrix initialized by ortho function

GLuint VaoId;
GLuint VboId[2];

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

Scene* scene = NULL;
int RES_X, RES_Y;

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode = 2;
bool antiAliasingEnabled = false;

int WindowHandle = 0;

///////////////////////////////////////////////////////////////////////  RAY-TRACE SCENE
Ray computeReflectionRay(Ray &ray, vec3 &normal, vec3 &hitpoint) {
	float c1 = DotProduct(normal, ray.direction);
	vec3 reflectDirection = ray.direction - (2 * normal * c1);

	reflectDirection.Normalize();

	return Ray(hitpoint + reflectDirection * 0.0001f, reflectDirection);
}
Ray computeRefractionRay(Ray &ray, vec3 &normal, vec3 &hitpoint, float incidingIndex, float refractingIndex) {
	Ray refractedRay;
	float n = incidingIndex / refractingIndex;

	float cosI =  DotProduct(normal, -1 * ray.direction);
	float cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);

	if (cosT2 > 0) {
		vec3 refractedDirection = ray.direction * n + (normal * (n * cosI - sqrtf(cosT2)));

		refractedDirection.Normalize();

		return Ray(hitpoint + refractedDirection * 0.00001f, refractedDirection);
	}
}
vec3 rayTracing(Ray &ray, int depth, float RefrIndex)
{
	vec3 color = vec3();

	bool hit = false;
	float closestDistance = FLT_MAX;
	vec3 closestHitpoint;
	Object* closestObj = nullptr;

	float distance;
	vec3 hitpoint;
	
	const std::vector<Object*> objs = scene->GetObjects();
	//intersect Ray with all objects and find a hit point(if any) closest to the start of the Ray
	for (auto &obj : objs) {
		if (obj->CheckRayCollision(ray, &distance, &hitpoint) == true) {
			
			if (distance < closestDistance) {
				hit = true;
				closestDistance = distance;
				closestObj = obj;
				closestHitpoint = hitpoint;
			}
		}
	}
	if (hit == false)
		return scene->GetBackgroundColor();
	else {
		vec3 normal;
		vec3 l;
		
		normal = closestObj->GetNormal(ray, closestHitpoint); //compute normal at the hit point;
		const std::vector<Light*> lights = scene->GetLights();
		Ray shadowFiller;
		for (auto &Light : lights) {
			
			l = Light->ComputeL(closestHitpoint); //unit Light vector from hit point to Light source
			shadowFiller = Ray(closestHitpoint + l * 0.0001f, l); //offset the hitpoint to avoid self intersection
			hit = false;
			//trace shadow Ray
			if (DotProduct(normal, l) > 0) {
				for (auto &obj : objs) {
					//check if Object is in shadow by checking if Light Ray collides with any Object
					if (obj->CheckRayCollision(shadowFiller, nullptr, nullptr) == true) {
						hit = true;
						break;
					}
				}
				//if not in shadow add Light contributtion to the color of the Object
				if (hit == false) {
					color += closestObj->GetDiffuseColor(*Light, normal, l);
					color += closestObj->GetSpecularColor(*Light, normal, l, -1 * ray.direction);
				}
			}
		}
		if (depth >= MAX_DEPTH) return color;
		
		if (closestObj->isReflective()) {
			Ray reflectiveRay = computeReflectionRay(ray, normal, closestHitpoint);
			vec3 rColor = rayTracing(reflectiveRay, depth + 1 , closestObj->mat.refraction_index);
			color += rColor * closestObj->GetReflectance();
		}
		
		if (closestObj->isTransmissive()) {
			Ray refractedRay = computeRefractionRay(ray, normal, closestHitpoint, RefrIndex, closestObj->GetEnterRefractionIndex(ray));
			vec3 tColor = rayTracing(refractedRay, depth + 1, closestObj->GetExitRefractionIndex(ray));
			color += tColor * closestObj->GetTransmittance();
	
		}
		
	}
	return color;
}

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
		exit(EXIT_FAILURE);
	}
}
/////////////////////////////////////////////////////////////////////// SHADERs
const GLchar* VertexShader =
{
	"#version 330 core\n"

	"in vec2 in_Position;\n"
	"in vec3 in_Color;\n"
	"uniform mat4 Matrix;\n"
	"out vec4 color;\n"

	"void main(void)\n"
	"{\n"
	"	vec4 position = vec4(in_Position, 0.0, 1.0);\n"
	"	color = vec4(in_Color, 1.0);\n"
	"	gl_Position = Matrix * position;\n"

	"}\n"
};

const GLchar* FragmentShader =
{
	"#version 330 core\n"

	"in vec4 color;\n"
	"out vec4 out_Color;\n"

	"void main(void)\n"
	"{\n"
	"	out_Color = color;\n"
	"}\n"
};

void createShaderProgram()
{
	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, 0);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");
	glBindAttribLocation(ProgramId, COLOR_ATTRIB, "in_Color");

	glLinkProgram(ProgramId);
	UniformId = glGetUniformLocation(ProgramId, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);
	glDeleteProgram(ProgramId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}
/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	
	/* N�o � necess�rio fazer glBufferData, ou seja o envio dos pontos para a placa gr�fica pois isso
	� feito na drawPoints em tempo de execu��o com GL_DYNAMIC_DRAW */

	glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glVertexAttribPointer(VERTEX_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);
	

	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glEnableVertexAttribArray(COLOR_ATTRIB);
	glVertexAttribPointer(COLOR_ATTRIB, 3, GL_FLOAT, 0, 0, 0);
	// unbind the VAO

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
	glDisableVertexAttribArray(COLOR_ATTRIB);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

void drawPoints()
{
	glBindVertexArray(VaoId);
	glUseProgram(ProgramId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, size_vertices, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ARRAY_BUFFER, size_colors, colors, GL_DYNAMIC_DRAW);

	glUniformMatrix4fv(UniformId, 1, GL_FALSE, m);
	glDrawArrays(GL_POINTS, 0, RES_X*RES_Y);
	glFinish();

	glUseProgram(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

// Render function by primary Ray casting from the eye towards the scene's objects

void renderScene()
{
	int index_pos = 0;
	int index_col = 0;

	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{
			
			vec3 color = vec3();
			if (antiAliasingEnabled) {
				for (int r = 0; r < ANTI_ALISING_NUMBER; r++)
				{
					Ray Ray = scene->GetCamera().getRandomPrimaryRay(x, y);
					color += rayTracing(Ray, 1, 1.0);
				}
			}
			Ray Ray = scene->GetCamera().getPrimaryRay(x, y);
			
			color += rayTracing(Ray, 1, 1.0);
			if (antiAliasingEnabled) 
				color /= ANTI_ALISING_NUMBER+1;

			vertices[index_pos++] = (float)x;
			vertices[index_pos++] = (float)y;
			colors[index_col++] = color.x;
			colors[index_col++] = color.y;
			colors[index_col++] = color.z;

			if (draw_mode == 0) {  // desenhar o conte�do da janela ponto a ponto
				drawPoints();
				index_pos = 0;
				index_col = 0;
			}
		}
		printf("line %d", y);
		if (draw_mode == 1) {  // desenhar o conte�do da janela linha a linha
			drawPoints();
			index_pos = 0;
			index_col = 0;
		}
	}

	if (draw_mode == 2) //preenchar o conte�do da janela com uma imagem completa
		drawPoints();

	printf("Terminou!\n");

}

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void ortho(float left, float right, float bottom, float top,
	float nearp, float farp)
{
	m[0 * 4 + 0] = 2 / (right - left);
	m[0 * 4 + 1] = 0.0;
	m[0 * 4 + 2] = 0.0;
	m[0 * 4 + 3] = 0.0;
	m[1 * 4 + 0] = 0.0;
	m[1 * 4 + 1] = 2 / (top - bottom);
	m[1 * 4 + 2] = 0.0;
	m[1 * 4 + 3] = 0.0;
	m[2 * 4 + 0] = 0.0;
	m[2 * 4 + 1] = 0.0;
	m[2 * 4 + 2] = -2 / (farp - nearp);
	m[2 * 4 + 3] = 0.0;
	m[3 * 4 + 0] = -(right + left) / (right - left);
	m[3 * 4 + 1] = -(top + bottom) / (top - bottom);
	m[3 * 4 + 2] = -(farp + nearp) / (farp - nearp);
	m[3 * 4 + 3] = 1.0;
}

void reshape(int w, int h)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, w, h);
	ortho(0, (float)RES_X, 0, (float)RES_Y, -1.0, 1.0);
}

/////////////////////////////////////////////////////////////////////// SETUP
void setupCallbacks()
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowPosition(640, 100);
	glutInitWindowSize(RES_X, RES_Y);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glDisable(GL_DEPTH_TEST);
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
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	createShaderProgram();
	createBufferObjects();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	
	scene = new Scene();
	if (!(scene->LoadSceneNFF("scenes/mount_very_high.nff"))) return 0;
	RES_X = scene->GetCamera().resolutionX;
	RES_Y = scene->GetCamera().resolutionY;

	if (draw_mode == 0) { // desenhar o conte�do da janela ponto a ponto
		size_vertices = 2 * sizeof(float);
		size_colors = 3 * sizeof(float);
		printf("DRAWING MODE: POINT BY POINT\n");
	}
	else if (draw_mode == 1) { // desenhar o conte�do da janela linha a linha
		size_vertices = 2 * RES_X * sizeof(float);
		size_colors = 3 * RES_X * sizeof(float);
		printf("DRAWING MODE: LINE BY LINE\n");
	}
	else if (draw_mode == 2) { // preencher o conte�do da janela com uma imagem completa
		size_vertices = 2 * RES_X*RES_Y * sizeof(float);
		size_colors = 3 * RES_X*RES_Y * sizeof(float);
		printf("DRAWING MODE: FULL IMAGE\n");
	}
	else {
		printf("Draw mode not valid \n");
		exit(0);
	}
	printf("resx = %d  resy= %d.\n", RES_X, RES_Y);

	vertices = (float*)malloc(size_vertices);
	if (vertices == NULL) exit(1);

	colors = (float*)malloc(size_colors);
	if (colors == NULL) exit(1);

	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}
///////////////////////////////////////////////////////////////////////