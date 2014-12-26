#include "OpenGLWorld.h"


OpenGLWorld::OpenGLWorld(void) : World()
{
	light = NULL;
}


OpenGLWorld::~OpenGLWorld(void)
{
	if (light != NULL)
		delete light;
}


void OpenGLWorld::Init3d()
{
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
		return;
	}

#ifdef USE_OPENGL
	if (!GLEW_VERSION_1_5)
		USE_LEGACY = true;
#endif

	printf("%s\n", glGetString(GL_VERSION));
	printf("1.5 %d\n", GL_VERSION_1_5);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);

    lightPosition[0] = -3.0f;
    lightPosition[1] = 3.0f;
    lightPosition[2] = -3.0f;
    lightPosition[3] = 0.0f;

    if (!USE_LEGACY)
	{
		standardShader = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
		depthShader = LoadShaders("DepthRTT.vertexshader", "DepthRTT.fragmentshader");
		textureViewerShader = LoadShaders("Passthrough.vertexshader", "SimpleTexture.fragmentshader");

		light = new Light();
		//camera.BuildProjection(false);
		//camera.SetPosition(RDRVEC3(lightPosition[0], lightPosition[1], lightPosition[2]));
		//camera.SetLookAt(RDRVEC3(-0.7f, 0.0f, 5.0f));
		light->SetPosition(RDRVEC3(lightPosition[0], lightPosition[1], lightPosition[2]));
		light->SetLookAt(RDRVEC3(-0.7f, 0.0f, 5.0f));
		light->InitShadow();
		light->InitQuad();
		light->PreShadow();
		//glUseProgram(standardShader);
		//glUniform3f(lightPositionId, lightPosition[0], lightPosition[1], lightPosition[2]);
	}
	else
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		lightPosition[2] *= -1;
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

		float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);

		float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

		//float lightDirection[] = { -2.0f, -2.0f, -3.0f };
		//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

		float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.125f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	}
}

void OpenGLWorld::PreRender()
{
	for (size_t i = 0; i < DrawableObjects.size(); i++)
	{

	}
}

void OpenGLWorld::PreUpdate()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLWorld::PostUpdate()
{
}

void OpenGLWorld::PostRender()
{
	if (!USE_LEGACY)
	{
		glDeleteProgram(standardShader);
		glDeleteProgram(depthShader);
	}
}

void OpenGLWorld::Draw()
{
	//// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthFrameBuffer());
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	PreUpdate();

	glUseProgram(depthShader);
	for (size_t i = 0; i < DrawableObjects.size(); i++)
    {
		DrawableObjects[i]->RenderShadow(light);
    }

	// Render to our screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	PreUpdate();

	glUseProgram(standardShader);
	World::Draw();

	// for debug shadow map texture
	glViewport(0, 0, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

	glUseProgram(textureViewerShader);
	light->DrawShadowTexture();
}
