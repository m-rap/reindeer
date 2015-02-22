#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Camera.h"
#include "Light.h"

class Drawable
{
public:
    Drawable();
    virtual ~Drawable();

	virtual void RenderShadow(Light* light) = 0;
    virtual void Draw(Camera* camera, Light* light) = 0;
};

#endif // DRAWABLE_H
