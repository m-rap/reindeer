#pragma once
#ifndef LEGACYOPENGLMODELRENDERER_H
#define LEGACYOPENGLMODELRENDERER_H

#include "BaseRenderer.h"
#include "Camera.h"

class LegacyOpenGLModelRenderer :
	public BaseRenderer
{
private:
	GLfloat* vertices;
	GLfloat* normals;
	GLbyte* indices;
	size_t vertexCount;

public:
	LegacyOpenGLModelRenderer(BaseObject* parent);
	virtual ~LegacyOpenGLModelRenderer(void);

	virtual void BuildBuffers();
	virtual void Draw(Camera* camera);
};

#endif