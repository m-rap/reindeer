#pragma once
#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "BaseRenderer.h"

class OpenGLRenderer :
	public BaseRenderer
{
protected:
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	GLuint uvBuffer;

	GLuint programId;
	GLuint positionId;
	GLuint normalId;
	GLuint mvpId;
	GLuint matrixId;
	GLuint viewId;
	GLuint viewInvId;
	GLuint normalMatId;
	GLuint textureId;
	GLuint uvId;

	GLuint texture;
public:
	OpenGLRenderer(BaseObject* parent, bool isIndexed = false, bool useTexture = false);
	virtual ~OpenGLRenderer(void);

	virtual void SetProgramId(const GLuint& programId);
	virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	);
	virtual void Draw(Camera* camera);
};

#endif