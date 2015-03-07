#include "LegacyOpenGLLight.h"

LegacyOpenGLLight::LegacyOpenGLLight(void) : Light()
{
	depthTexture = 0;
}

LegacyOpenGLLight::~LegacyOpenGLLight(void)
{
	if (depthTexture != 0)
		glDeleteTextures(1, &depthTexture);
}

void LegacyOpenGLLight::SetDepthShader(const GLuint& id)
{
}

void LegacyOpenGLLight::SetStandardShader(const GLuint& id)
{
}

void LegacyOpenGLLight::SetTextureViewerShader(const GLuint& id)
{
}

GLuint LegacyOpenGLLight::GetDepthTexture()
{
	return depthTexture;
}

GLuint LegacyOpenGLLight::GetDepthFrameBuffer()
{
	return 0;
}

void LegacyOpenGLLight::Init()
{
	InitShadowMap();
}

bool LegacyOpenGLLight::InitShadowMap()
{
	glGenTextures(1, &depthTexture); // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, DEPTHTEX_WIDTH, DEPTHTEX_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	return true;
}

void LegacyOpenGLLight::DrawShadowMapTexture()
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
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quadVertices);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

void LegacyOpenGLLight::RenderLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float lightPosition[4];
	lightPosition[0] = position.x;
	lightPosition[1] = position.y;
	lightPosition[2] = -position.z;
	lightPosition[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT0, GL_POSITION, &position.x);

	float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient); // we use scene ambient instead of per light ambient

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

void LegacyOpenGLLight::RenderDimLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	float lightPosition[4];
	lightPosition[0] = position.x;
	lightPosition[1] = position.y;
	lightPosition[2] = -position.z;
	lightPosition[3] = 0.0f;
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT0, GL_POSITION, &position.x);

	float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);

	float lightDiffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}

void LegacyOpenGLLight::ApplyShadowMap(Camera& c)
{
	glActiveTexture(GL_TEXTURE1); glEnable(GL_TEXTURE_2D); // enables texture 1
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glm::mat4& projection = *(c.GetProjection());
	glm::mat4& view = *(c.GetView());

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(view));

	glm::mat4 textureMatrix = GetDepthBiasMVP(glm::mat4(1.0));

	glEnable(GL_TEXTURE_GEN_S);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_S, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][0], textureMatrix[1][0], textureMatrix[2][0], textureMatrix[3][0])));

	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_T, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][1], textureMatrix[1][1], textureMatrix[2][1], textureMatrix[3][1])));

	glEnable(GL_TEXTURE_GEN_R);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_R, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][2], textureMatrix[1][2], textureMatrix[2][2], textureMatrix[3][2])));

	glEnable(GL_TEXTURE_GEN_Q);
	glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
	glTexGenfv(GL_Q, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][3], textureMatrix[1][3], textureMatrix[2][3], textureMatrix[3][3])));

	//Enable shadow comparison
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//Shadow comparison should be true (ie not in shadow) if r<=texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//Shadow comparison should generate an INTENSITY result
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.99f);
}