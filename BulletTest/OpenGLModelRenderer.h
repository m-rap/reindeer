#pragma once
#ifndef OPENGLMODELRENDERER_H
#define OPENGLMODELRENDERER_H

#include "ModelRenderer.h"

class OpenGLModelRenderer :
	public ModelRenderer
{
private:
	GLuint vertexBuffer;
	GLuint normalBuffer;

	GLuint programId;
	GLuint positionId;
	GLuint normalId;
	GLuint mvpId;
	GLuint matrixId;
	GLuint viewId;
	GLuint viewInvId;
	GLuint normalMatId;

	size_t vertexCount;
public:
	OpenGLModelRenderer(ModelObject* parent);
	virtual ~OpenGLModelRenderer(void);

	virtual void SetProgramId(const GLuint& programId);
	virtual void BuildBuffers();
	virtual void Draw(Camera* camera);
};

#endif