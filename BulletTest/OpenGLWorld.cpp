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
	if (!USE_LEGACY)
		glBindFramebuffer(GL_FRAMEBUFFER, light->GetDepthFrameBuffer()); // Render to our framebuffer
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
	}

	if (!USE_LEGACY)
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // Render to our screen
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	PreUpdate();

	if (!USE_LEGACY) {
		glUseProgram(standardShader);
    } else {
        glEnable(GL_TEXTURE_2D);
        // shadow related
        glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
        );
        glm::mat4 lightProjection = *light->GetProjection();
        glm::mat4 lightView = *light->GetView();
        glm::mat4 textureMatrix = biasMatrix * lightProjection * lightView;

        glEnable(GL_TEXTURE_GEN_S);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGenfv(GL_S, GL_EYE_PLANE, glm::value_ptr(textureMatrix[0]));

        glEnable(GL_TEXTURE_GEN_T);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGenfv(GL_T, GL_EYE_PLANE, glm::value_ptr(textureMatrix[1]));

        glEnable(GL_TEXTURE_GEN_R);
        glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGenfv(GL_R, GL_EYE_PLANE, glm::value_ptr(textureMatrix[2]));

        glEnable(GL_TEXTURE_GEN_Q);
        glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        glTexGenfv(GL_Q, GL_EYE_PLANE, glm::value_ptr(textureMatrix[3]));

        /*float row1[4] = {textureMatrix[0][0], textureMatrix[1][0], textureMatrix[2][0], textureMatrix[3][0]},
              row2[4] = {textureMatrix[0][1], textureMatrix[1][1], textureMatrix[2][1], textureMatrix[3][1]},
              row3[4] = {textureMatrix[0][2], textureMatrix[1][2], textureMatrix[2][2], textureMatrix[3][2]},
              row4[4] = {textureMatrix[0][3], textureMatrix[1][3], textureMatrix[2][3], textureMatrix[3][3]};*/

        //glEnable(GL_TEXTURE_GEN_S);
        //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_S, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][0], textureMatrix[1][0], textureMatrix[2][0], textureMatrix[3][0])));
        //
        //glEnable(GL_TEXTURE_GEN_T);
        //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_T, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][1], textureMatrix[1][1], textureMatrix[2][1], textureMatrix[3][1])));
        //
        //glEnable(GL_TEXTURE_GEN_R);
        //glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_R, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][2], textureMatrix[1][2], textureMatrix[2][2], textureMatrix[3][2])));
        //
        //glEnable(GL_TEXTURE_GEN_Q);
        //glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_Q, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][3], textureMatrix[1][3], textureMatrix[2][3], textureMatrix[3][3])));

        //glEnable(GL_TEXTURE_GEN_S);
        //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_S, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][0], textureMatrix[0][1], textureMatrix[0][2], textureMatrix[0][3])));
        //
        //glEnable(GL_TEXTURE_GEN_T);
        //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_T, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[1][0], textureMatrix[1][1], textureMatrix[1][2], textureMatrix[1][3])));
        //
        //glEnable(GL_TEXTURE_GEN_R);
        //glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_R, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[2][0], textureMatrix[2][1], textureMatrix[2][2], textureMatrix[2][3])));
        //
        //glEnable(GL_TEXTURE_GEN_Q);
        //glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
        //glTexGenfv(GL_Q, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[3][0], textureMatrix[3][1], textureMatrix[3][2], textureMatrix[3][3])));

        //glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, light->GetDepthTexture());

        //Enable shadow comparison
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
        //Shadow comparison should be true (ie not in shadow) if r<=texture
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        //Shadow comparison should generate an INTENSITY result
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GEQUAL, 0.99f);
    }

	light->RenderLighting();
	World::Draw();

	if (USE_LEGACY) {
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_ALPHA_TEST);
        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);
        glDisable(GL_TEXTURE_GEN_R);
        glDisable(GL_TEXTURE_GEN_Q);
	}

	//// for debug shadow map texture
	//glViewport(0, 0, DEPTHTEX_WIDTH / 4, DEPTHTEX_HEIGHT / 4);
    //
	//if (!USE_LEGACY)
	//	glUseProgram(textureViewerShader);
	//light->DrawShadowMapTexture();
}
