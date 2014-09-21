#ifndef LEGACYOPENGLBOXRENDERER_H
#define LEGACYOPENGLBOXRENDERER_H

#include "BoxRenderer.h"

class LegacyOpenGLBoxRenderer :
	public BoxRenderer
{
private:
	GLfloat vertices[72];
	GLfloat normals[72];
	GLbyte indices[36];
public:
	LegacyOpenGLBoxRenderer(BoxObject* parent);
	virtual ~LegacyOpenGLBoxRenderer();

	virtual void SetProgramId(const GLuint& programId);
	virtual void BuildBuffers();
	virtual void BuildNormalsIndices();
	virtual void Draw(Camera* camera);
};

#endif // LEGACYOPENGLBOXRENDERER_H
