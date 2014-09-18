#ifndef BOXRENDERER_H
#define BOXRENDERER_H

#include "Camera.h"

class BoxRenderer
{
public:
	BoxRenderer();
	virtual ~BoxRenderer();

	virtual void BuildWidthHeightLength() = 0;
	virtual void BuildBuffers() = 0;
	virtual void BuildNormalsIndices() = 0;
	virtual void Draw(Camera& camera) = 0;
};

#endif // BOXOBJECTIMP_H
