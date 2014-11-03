#pragma once
#ifndef LEGACYOPENGLRENDERER_H
#define LEGACYOPENGLRENDERER_H

#include "BaseRenderer.h"
#include "Camera.h"

class LegacyOpenGLRenderer :
	public BaseRenderer
{
protected:
	GLfloat* vertices;
	GLfloat* normals;
	GLushort* indices;
	GLfloat* uvs;

	GLuint texture;

public:
	LegacyOpenGLRenderer(BaseObject* parent, bool isIndexed = false, bool useTexture = false);
	virtual ~LegacyOpenGLRenderer(void);

	virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	);
	virtual void Draw(Camera* camera);
};

#endif