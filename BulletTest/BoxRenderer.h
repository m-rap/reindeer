#ifndef BOXRENDERER_H
#define BOXRENDERER_H

#include "BaseRenderer.h"

class BoxRenderer :
	public BaseRenderer
{
public:
	BoxRenderer(BaseObject* parent);
	virtual ~BoxRenderer();

	virtual void BuildNormalsIndices() = 0;
};

#endif // BOXOBJECTIMP_H
