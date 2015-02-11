#include "Light.h"
#include "OpenGLWorld.h"

Light::Light(void)
{
#ifdef USE_OPENGL
	depthFrameBuffer = 0;
	depthTexture = 0;

	float s = 0.25f;
	projection = glm::ortho<float>(-10*s, 10*s, -10*s, 10*s, -10, 20);
	if (!USE_LEGACY)
        glGenBuffers(1, &quadVertexBuffer);
#endif
}

Light::~Light(void)
{
	if (depthFrameBuffer != 0)
		glDeleteFramebuffers(1, &depthFrameBuffer);
	if (depthTexture != 0)
		glDeleteTextures(1, &depthTexture);
	glDeleteBuffers(1, &quadVertexBuffer);
}

#ifdef USE_OPENGL
void Light::SetDepthShader(const GLuint& id)
{
	this->depthShader = id;
	this->depthMVPId = glGetUniformLocation(depthShader, "depthMVP");
}

void Light::SetStandardShader(const GLuint& id)
{
	this->standardShader = id;
	this->lightPositionId = glGetUniformLocation(standardShader, "LightPosition_worldspace");
}

void Light::SetTextureViewerShader(const GLuint& id)
{
	this->textureViewerShader = id;
	this->textureViewerVertexId = glGetAttribLocation(id, "vertexPosition_modelspace");
	this->textureViewerTextureId = glGetUniformLocation(id, "texture");
}

GLuint Light::GetDepthTexture()
{
	return depthTexture;
}

GLuint Light::GetDepthFrameBuffer()
{
	return depthFrameBuffer;
}
#endif

void Light::Init()
{
	if (!USE_LEGACY)
	{
		SetStandardShader(((OpenGLWorld*)World::Global)->standardShader);
		SetDepthShader(((OpenGLWorld*)World::Global)->depthShader);
		SetTextureViewerShader(((OpenGLWorld*)World::Global)->textureViewerShader);

		static const GLfloat g_quad_vertex_buffer_data[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
		};

		glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	}
	else
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		//lightPosition[2] *= -1;
		glLightfv(GL_LIGHT0, GL_POSITION, &position.x);

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

	InitShadowMap();
}

bool Light::InitShadowMap()
{
	if (!USE_LEGACY) {
        glGenFramebuffers(1, &depthFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);
    }

    glGenTextures(1, &depthTexture); // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

	if (!USE_LEGACY) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

		// No color output in the bound framebuffer, only depth.
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	} else {
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT, GL_DEPTH_COMPONENT24, GL_UNSIGNED_BYTE, 0);

	// Always check that our framebuffer is ok
	if (!USE_LEGACY) {
        GLuint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(error != GL_FRAMEBUFFER_COMPLETE)
            return false;
    }

	return true;
}

void Light::BuildDepthMVP()
{
#ifdef USE_OPENGL
	view = glm::lookAt(position, lookAt, glm::vec3(0,1,0));
#endif
}

void Light::SetPosition(const RDRVEC3& position, bool silent)
{
	BaseObject::SetPosition(position, silent);
	if (!silent)
		BuildDepthMVP();
}

void Light::SetLookAt(const RDRVEC3& lookAt, bool silent)
{
	this->lookAt = lookAt;
	if (!silent)
		BuildDepthMVP();
}

RDRMAT4 Light::GetDepthMVP(const glm::mat4& world)
{
	return projection * view * world;
}

RDRMAT4 Light::GetDepthBiasMVP(const glm::mat4& depthMVP)
{
#ifdef USE_OPENGL
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
    return biasMatrix * depthMVP;
#endif
}

void Light::DrawShadowMapTexture()
{
	if (!USE_LEGACY)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(textureViewerTextureId, 0);
		glEnableVertexAttribArray(textureViewerVertexId);
		glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
		glVertexAttribPointer(
			textureViewerVertexId,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,//sizeof(GL_FLOAT) * 3,
			(void*)0
		);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(textureViewerVertexId);
	}
	else
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		static GLfloat uvs[] = {
			0.0f, 0.0f,
            1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		};
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
		static GLfloat quadVertices[] = {
			-1.0f, -1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			 1.0f, -1.0f, 0.0f,
			 1.0f,  1.0f, 0.0f,
		};
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, quadVertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_VERTEX_ARRAY);
		glDisable(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
	}
}

void Light::RenderLighting()
{
	if (!USE_LEGACY)
	{
	}
	else
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		//lightPosition[2] *= -1;
		glLightfv(GL_LIGHT0, GL_POSITION, &position.x);

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
