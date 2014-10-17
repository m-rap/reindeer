#ifndef OPENGLBOXRENDERER_H
#define OPENGLBOXRENDERER_H

#include "BoxRenderer.h"

class OpenGLBoxRenderer :
	public BoxRenderer
{
private:
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;

	GLuint programId;
	GLuint positionId;
	GLuint normalId;
	GLuint mvpId;
	GLuint matrixId;
	GLuint viewId;
	GLuint viewInvId;
	GLuint normalMatId;
public:
	OpenGLBoxRenderer(BaseObject* parent);
	virtual ~OpenGLBoxRenderer();

	virtual void SetProgramId(const GLuint& programId);
	virtual void BuildBuffers();
	virtual void BuildNormalsIndices();
	virtual void Draw(Camera* camera);
};

#endif // OPENGLBOXRENDERER_H
