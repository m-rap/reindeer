#include "Light.h"
#include "OpenGLWorld.h"

Light::Light(void)
{
#ifdef USE_OPENGL
	depthFrameBuffer = 0;
	depthTexture = 0;

	float s = 1.0f;
	projection = glm::ortho<float>(-10*s, 10*s, -10*s, 10*s, -10*s, 20*s);
	glGenBuffers(1, &quadVertexBuffer);

	SetStandardShader(((OpenGLWorld*)World::Global)->standardShader);
	SetDepthShader(((OpenGLWorld*)World::Global)->depthShader);
	SetTextureViewerShader(((OpenGLWorld*)World::Global)->textureViewerShader);
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

void Light::InitQuad()
{
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

void Light::InitShadow()
{
	glGenFramebuffers(1, &depthFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
}

bool Light::PreShadow()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, SCREEN_WIDTH,SCREEN_HEIGHT, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Always check that our framebuffer is ok
	GLuint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(error != GL_FRAMEBUFFER_COMPLETE)
		return false;

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

void Light::DrawShadowTexture()
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
