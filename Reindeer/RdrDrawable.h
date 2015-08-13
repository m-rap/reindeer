#ifndef RDRDRAWABLE_H
#define RDRDRAWABLE_H

#include "RdrTransform.h"

class RdrCamera;
class RdrLight;

class RdrDrawable
{
public:
    RdrDrawable();
    virtual ~RdrDrawable();

	virtual void RenderShadow(RdrTransform* tr, RdrLight* light) = 0;
    virtual void Draw(RdrTransform* tr, RdrCamera* camera, RdrLight* light) = 0;
};

#endif // RDRDRAWABLE_H
