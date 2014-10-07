#ifndef BOXRENDERER_H
#define BOXRENDERER_H

#include "Camera.h"

class BoxObject;

class BoxRenderer
{
protected:
	BoxObject* parent;
public:
	BoxRenderer(BoxObject* parent);
	virtual ~BoxRenderer();

#ifdef USE_OPENGL
	virtual void SetProgramId(const GLuint& programId) = 0;
#endif
	virtual void BuildBuffers() = 0;
	virtual void BuildNormalsIndices() = 0;
	virtual void Draw(Camera* camera) = 0;
};

#endif // BOXOBJECTIMP_H