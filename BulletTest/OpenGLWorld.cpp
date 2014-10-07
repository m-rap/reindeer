#include "OpenGLWorld.h"


OpenGLWorld::OpenGLWorld(void) : World()
{
}


OpenGLWorld::~OpenGLWorld(void)
{
}


void OpenGLWorld::Init3d()
{	
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
		return;
	}

	/*if (!GLEW_VERSION_1_5) {
		printf("glGenBuffers is not supported\n");
		return;
	}*/

	printf("%s\n", glGetString(GL_VERSION));
	printf("1.5 %d\n", GL_VERSION_1_5);

	
    lightPosition[0] = -3.0f;
    lightPosition[1] = 3.0f;
    lightPosition[2] = -3.0f;
    lightPosition[3] = 0.0f;

    if (GLEW_VERSION_1_5)
	{
		programId = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
		lightPositionId = glGetUniformLocation(programId, "LightPosition_worldspace");
		glUseProgram(programId);
		glUniform3f(lightPositionId, lightPosition[0], lightPosition[1], lightPosition[2]);
	}
	else
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		//float lightAmbient[] = { 50.0f/255.0f, 50.0f/255.0f, 50.0f/255.0f, 1.0f };
		float lightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		//float lightDirection[] = { -2.0f, -2.0f, -3.0f };
		//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		lightPosition[2] *= -1;
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.125f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

		float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
}


void OpenGLWorld::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLWorld::PostUpdate()
{
}

void OpenGLWorld::PostRender()
{
	if (GLEW_VERSION_1_5)
	{
		glDeleteProgram(programId);
	}
}