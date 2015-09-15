#include "RdrWorld_OpenGL.h"
#include "RdrLight_LegacyOpenGL.h"
#include "RdrLight_OpenGL.h"

RdrWorld_OpenGL::RdrWorld_OpenGL(Container* container) : RdrWorld(container)
{
	light = NULL;
}


RdrWorld_OpenGL::~RdrWorld_OpenGL(void)
{
	if (light != NULL)
		delete light;
}


void RdrWorld_OpenGL::Init3d()
{
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
		return;
	}

	if (!GLEW_VERSION_1_5)
		USE_LEGACY = true;

	printf("%s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	//glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);

	if (!USE_LEGACY) {
		standardShader = LoadShaders("shader/StandardShading.vert", "shader/StandardShading.frag");
		depthShader = LoadShaders("shader/DepthRTT.vert", "shader/DepthRTT.frag");
		textureViewerShader = LoadShaders("shader/Passthrough.vert", "shader/SimpleTexture.frag");
		light = new RdrLight_OpenGL();
	}
	else {
		light = new RdrLight_LegacyOpenGL();
	}

	light->GetTransform()->SetPosition(RDRVEC3(3.0f, 3.0f, 7.0f));
	light->GetTransform()->LookAt(RDRVEC3(-0.7f, 1.0f, 0.5f));
	light->BuildView();
	light->Init();
}

void RdrWorld_OpenGL::PreShadow()
{
    if (!USE_LEGACY)
		glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthFrameBuffer()); // Render to our framebuffer

	glViewport(0, 0, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    if (!USE_LEGACY) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glShadeModel(GL_SMOOTH);
        glColorMask(1, 1, 1, 1);

        glUseProgram(depthShader);
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        glShadeModel(GL_SMOOTH);
        glColorMask(0, 0, 0, 0);
    }
}

void RdrWorld_OpenGL::PostShadow() {
    if (USE_LEGACY) {
		glBindTexture(GL_TEXTURE_2D, light->GetDepthTexture());
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT);
	}
}

void RdrWorld_OpenGL::PreDraw()
{
    if (!USE_LEGACY)
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Render to our screen

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    if (!USE_LEGACY) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glShadeModel(GL_SMOOTH);
        glColorMask(1, 1, 1, 1);
        glUseProgram(standardShader);
    } else {
        light->RenderDimLight();
        RdrWorld::Draw();
    }

    light->ApplyShadowMap(camera);
    light->RenderLight();
}

void RdrWorld_OpenGL::PostDraw()
{
    if (USE_LEGACY) {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

        glActiveTexture(GL_TEXTURE1);glDisable(GL_TEXTURE_2D); // disables texture 1
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
        glDisable(GL_TEXTURE_GEN_Q);
	}
}

void RdrWorld_OpenGL::PostRender()
{
	if (!USE_LEGACY)
	{
		glDeleteProgram(standardShader);
		glDeleteProgram(depthShader);
	}
}

void RdrWorld_OpenGL::Draw()
{
	PreShadow();
	for (deque<RdrNode*>::iterator it = nodes.begin(); it != nodes.end(); it++) {
		(*it)->RenderShadow(NULL, light);
    }
    PostShadow();
	PreDraw();
    RdrWorld::Draw();
	PostUpdate();

	if (DEBUG_SHADOWMAP) { // for debug shadow map texture
        glViewport(0, 0, DEPTHTEX_WIDTH / 4, DEPTHTEX_HEIGHT / 4);
        if (!USE_LEGACY)
            glUseProgram(textureViewerShader);
        light->DrawShadowMapTexture();
	}
}
