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

	//if (!GLEW_VERSION_1_5)
		USE_LEGACY = true;

	printf("%s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);

	if (!USE_LEGACY)
	{
		standardShader = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
		depthShader = LoadShaders("DepthRTT.vertexshader", "DepthRTT.fragmentshader");
		textureViewerShader = LoadShaders("Passthrough.vertexshader", "SimpleTexture.fragmentshader");
	}

	light = new Light();
	light->SetPosition(RDRVEC3(-3.0f, 3.0f, -3.0f));
	light->SetLookAt(RDRVEC3(-0.7f, 0.0f, 5.0f));
	light->Init();
}

void OpenGLWorld::PreRender()
{
	for (size_t i = 0; i < DrawableObjects.size(); i++) {

	}
}

void OpenGLWorld::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glColorMask(1, 1, 1, 1);
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
	if (!USE_LEGACY)
		glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthFrameBuffer());
	glViewport(0, 0, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	if (!USE_LEGACY)
		PreUpdate();
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		glShadeModel(GL_SMOOTH);
		glColorMask(0, 0, 0, 0);
	}

	if (!USE_LEGACY)
		glUseProgram(depthShader);
	for (size_t i = 0; i < DrawableObjects.size(); i++) {
		DrawableObjects[i]->RenderShadow(light);
    }
	if (USE_LEGACY) {
		glBindTexture(GL_TEXTURE_2D, light->GetDepthTexture());
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT);
		//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 0, 0, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT, 0);
	}

	// Render to our screen
	if (!USE_LEGACY)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	PreUpdate();

	if (!USE_LEGACY)
		glUseProgram(standardShader);

	light->RenderLighting();
	World::Draw();

	if (USE_LEGACY) {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}

	// for debug shadow map texture
	glViewport(0, 0, DEPTHTEX_WIDTH / 4, DEPTHTEX_HEIGHT / 4);

	if (!USE_LEGACY)
		glUseProgram(textureViewerShader);
	light->DrawShadowMapTexture();
}
