#include "RdrLight_OpenGL.h"
#include "RdrWorld_OpenGL.h"

RdrLight_OpenGL::RdrLight_OpenGL(void) : RdrLight()
{
	depthFrameBuffer = 0;
	depthTexture = 0;

	float s = 0.25f;
	projection = glm::ortho<float>(-10 * s, 10 * s, -10 * s, 10 * s, 0, 20);

	glGenBuffers(1, &quadVertexBuffer);
}

RdrLight_OpenGL::~RdrLight_OpenGL(void)
{
	if (depthFrameBuffer != 0)
		glDeleteFramebuffers(1, &depthFrameBuffer);
	if (depthTexture != 0)
		glDeleteTextures(1, &depthTexture);
	glDeleteBuffers(1, &quadVertexBuffer);
}

void RdrLight_OpenGL::SetDepthShader(const GLuint& id)
{
	this->depthShader = id;
	this->depthMVPId = glGetUniformLocation(depthShader, "depthMVP");
}

void RdrLight_OpenGL::SetStandardShader(const GLuint& id)
{
	this->standardShader = id;
	this->lightPositionId = glGetUniformLocation(standardShader, "LightPosition_worldspace");
}

void RdrLight_OpenGL::SetTextureViewerShader(const GLuint& id)
{
	this->textureViewerShader = id;
	this->textureViewerVertexId = glGetAttribLocation(id, "vertexPosition_modelspace");
	this->textureViewerTextureId = glGetUniformLocation(id, "texture");
}

GLuint RdrLight_OpenGL::GetDepthTexture()
{
	return depthTexture;
}

GLuint RdrLight_OpenGL::GetDepthFrameBuffer()
{
	return depthFrameBuffer;
}

void RdrLight_OpenGL::Init()
{
	SetStandardShader(((RdrWorld_OpenGL*)RdrWorld::Global)->standardShader);
	SetDepthShader(((RdrWorld_OpenGL*)RdrWorld::Global)->depthShader);
	SetTextureViewerShader(((RdrWorld_OpenGL*)RdrWorld::Global)->textureViewerShader);

	static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	InitShadowMap();
}

bool RdrLight_OpenGL::InitShadowMap()
{
	glGenFramebuffers(1, &depthFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFrameBuffer);

	glGenTextures(1, &depthTexture); // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//#if !defined __arm__ && !defined __aarch64__
#ifndef USE_GLES
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
#endif

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

//#if !defined __arm__ && !defined __aarch64__
#ifndef USE_GLES
	// No color output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
#endif

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	// Always check that our framebuffer is ok
	GLuint error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (error != GL_FRAMEBUFFER_COMPLETE)
		return false;

	return true;
}

void RdrLight_OpenGL::DrawShadowMapTexture()
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

void RdrLight_OpenGL::RenderLight()
{
}

void RdrLight_OpenGL::RenderDimLight()
{
}

void RdrLight_OpenGL::ApplyShadowMap(RdrCamera& c)
{
}
