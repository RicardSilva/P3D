///////////////////////////////////////////////////////////////////////
//
// P3D Course
// (c) 2016 by João Madeiras Pereira
// TEMPLATE: Whitted Ray Tracing NFF scenes and drawing points with Modern OpenGL
//
//You should develop your rayTracing( Ray Ray, int depth, float RefrIndex) which returns a color and
// to develop your load_NFF function
//
///////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <chrono>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "scene.h"
#include "vec.h"
#include "Ray.h"
#include "grid.h"

#define CAPTION "Ray tracer"

#define VERTEX_COORD_ATTRIB 0
#define COLOR_ATTRIB 1

#define MAX_DEPTH 8
#define ANTI_ALIASING_NUMBER 4	// SQRT OF THE RAYS PER PIXEL IN ANTI-ALIASING MODE OF 1 AND 2
#define SHADOW_NUMBER 4			// SQRT OF THE NUMBER OF SHADOW FILLERS PER POINT IN SHADOW MODE 2 AND 3
								// IN SHADOW MODE 3 WE SHOULD HAVE ANTI_ALIASING_NUMBER == SHADOW_NUMBER!!!! 

/* Draw Mode: 0 - point by point; 1 - line by line; 2 - full frame */
int draw_mode = 2;
/* AntiAliasing Mode: 0 - no aliasing; 1 - iterative random aliasing; 2 - jittering aliasing */
int antiAliasing_mode = 2;
/* Shadows Mode: 0 - hardShadows; 1 - random soft shadows; 2 - iterative random soft shadows; 3 - soft jittering shadows */
int shadow_mode = 3;
int shadow_shuffle = 0;
/* Acceleration mode: 0 - no acceleration; 1 - grid based acceleration */
int acceleration_mode = 1;

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
Camera camera;
int RES_X, RES_Y;
std::vector <Light*> lights;
std::vector <Object*> objects;
Grid grid;


std::chrono::steady_clock::time_point begin;
std::chrono::steady_clock::time_point end;

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
	else {
		return Ray();
	}
}
vec3 rayTracing(Ray &ray, int depth, float RefrIndex)
{
	vec3 color = vec3();

	bool hit = false;
	float closestDistance = FLT_MAX;
	vec3 closestHitpoint;
	Object* closestObj = nullptr;

	if (acceleration_mode == 0) {
		float distance;
		vec3 hitpoint;
		//intersect Ray with all objects and find a hit point(if any) closest to the start of the Ray
		for (auto &obj : objects) {
			if (obj->CheckRayCollision(ray, &distance, &hitpoint) == true) {

				if (distance < closestDistance) {
					hit = true;
					closestDistance = distance;
					closestObj = obj;
					closestHitpoint = hitpoint;
				}
			}
		}
	}
	else if (acceleration_mode == 1) {
		hit = grid.Traverse(ray, &closestObj, &closestHitpoint);
	}
	if (hit == false)
		return scene->GetBackgroundColor();
	else {
		vec3 normal;
		vec3 light_direction;
		
		normal = closestObj->GetNormal(ray, closestHitpoint); //compute normal at the hit point;
		int shadow_number = 1;
		if(shadow_mode == 2){
			shadow_number = SHADOW_NUMBER*SHADOW_NUMBER;
		}
		
		Ray shadowFiller;
		for (auto &l : lights) {
			for (int s = 0; s < shadow_number; s++) {
				
				if (shadow_mode == 0) {
					light_direction = l->ComputeL(closestHitpoint);
				}
				else if (shadow_mode == 1 || shadow_mode == 2) {
					light_direction = l->ComputeRandomL(closestHitpoint);
				}
				else if (shadow_mode == 3) {
					light_direction = l->ComputeJitteringL(closestHitpoint,shadow_shuffle%SHADOW_NUMBER,(shadow_shuffle/SHADOW_NUMBER)%SHADOW_NUMBER,SHADOW_NUMBER); 
				}
				shadowFiller = Ray(closestHitpoint + light_direction * 0.0001f, light_direction); //offset the hitpoint to avoid self intersection
				hit = false;
				//trace shadow Ray
				if (DotProduct(normal, light_direction) > 0) {
					if (acceleration_mode == 0) {
						for (auto &obj : objects) {
							//check if Object is in shadow by checking if Light Ray collides with any Object
							if (obj->CheckRayCollision(shadowFiller, nullptr, nullptr) == true) {
								hit = true;
								break;
							}
						}
					}
					else if (acceleration_mode == 1) {
						hit = grid.Traverse(shadowFiller);
					}
					//if not in shadow add Light contributtion to the color of the Object
					if (hit == false) {
						color += closestObj->GetDiffuseColor(*l, normal, light_direction)/shadow_number;
						color += closestObj->GetSpecularColor(*l, normal, light_direction, -1 * ray.direction)/shadow_number;
					}
				}
			}
		}
		
		if (depth >= MAX_DEPTH) return color;
		
		if (closestObj->isReflective()) {
			Ray reflectiveRay = computeReflectionRay(ray, normal, closestHitpoint);
			vec3 rColor = rayTracing(reflectiveRay, depth + 1 , closestObj->GetExitRefractionIndex(ray));
			color += rColor * closestObj->GetReflectance();
		}
		
		if (closestObj->isTransmissive()) {
			Ray refractedRay = computeRefractionRay(ray, normal, closestHitpoint, RefrIndex, closestObj->GetEnterRefractionIndex(ray));
			if (!refractedRay.IsNull()) {
				vec3 tColor = rayTracing(refractedRay, depth + 1, closestObj->GetExitRefractionIndex(ray));
				color += tColor * closestObj->GetTransmittance();
			}
	
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
	
	/* Não é necessário fazer glBufferData, ou seja o envio dos pontos para a placa gráfica pois isso
	é feito na drawPoints em tempo de execução com GL_DYNAMIC_DRAW */

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
	begin = std::chrono::steady_clock::now();
	for (int y = 0; y < RES_Y; y++)
	{
		for (int x = 0; x < RES_X; x++)
		{
			vec3 color = vec3();

			if (antiAliasing_mode == 0) {
				Ray Ray = camera.getPrimaryRay(x, y);
				color += rayTracing(Ray, 1, 1.0);
			}
			else if (antiAliasing_mode == 1) {
				for (int r = 0; r < ANTI_ALIASING_NUMBER*ANTI_ALIASING_NUMBER; r++)
				{
					Ray Ray = camera.getRandomPrimaryRay(x, y);
					color += rayTracing(Ray, 1, 1.0);
				}
				Ray Ray = camera.getPrimaryRay(x, y);
				color += rayTracing(Ray, 1, 1.0);
				color /= ANTI_ALIASING_NUMBER*ANTI_ALIASING_NUMBER + 1;
			}			
			else if (antiAliasing_mode == 2) {
				for (int p = 0; p < ANTI_ALIASING_NUMBER; p++) {
					for (int q = 0; q < ANTI_ALIASING_NUMBER; q++) {
						Ray Ray = camera.getJitteredPrimaryRay(x, y, p, q, ANTI_ALIASING_NUMBER);
						color += rayTracing(Ray, 1, 1.0);
						if (shadow_mode == 3)
							shadow_shuffle++;
					}
				}
				color /= (ANTI_ALIASING_NUMBER * ANTI_ALIASING_NUMBER);
				if (shadow_mode == 3)
					shadow_shuffle++;
			}

			vertices[index_pos++] = (float)x;
			vertices[index_pos++] = (float)y;
			colors[index_col++] = color.x;
			colors[index_col++] = color.y;
			colors[index_col++] = color.z;

			if (draw_mode == 0) {  // desenhar o conteúdo da janela ponto a ponto
				drawPoints();
				index_pos = 0;
				index_col = 0;
			}
		}
		printf("line %d", y);
		if (draw_mode == 1) {  // desenhar o conteúdo da janela linha a linha
			drawPoints();
			index_pos = 0;
			index_col = 0;
		}
	}

	if (draw_mode == 2) //preenchar o conteúdo da janela com uma imagem completa
		drawPoints();

	end = std::chrono::steady_clock::now();
	std::cout << std::endl << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
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
	camera = scene->GetCamera();
	RES_X = camera.resolutionX;
	RES_Y = camera.resolutionY;
	objects = scene->GetObjects();
	lights = scene->GetLights();
	if (acceleration_mode == 1) {
		grid = Grid(objects);
	}
	if (draw_mode == 0) { // desenhar o conteúdo da janela ponto a ponto
		size_vertices = 2 * sizeof(float);
		size_colors = 3 * sizeof(float);
		printf("DRAWING MODE: POINT BY POINT\n");
	}
	else if (draw_mode == 1) { // desenhar o conteúdo da janela linha a linha
		size_vertices = 2 * RES_X * sizeof(float);
		size_colors = 3 * RES_X * sizeof(float);
		printf("DRAWING MODE: LINE BY LINE\n");
	}
	else if (draw_mode == 2) { // preencher o conteúdo da janela com uma imagem completa
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