#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "Camera.h"

class Drawable
{
public:
    Drawable();
    virtual ~Drawable();

    virtual void Draw(Camera& camera) = 0;
};

#endif // DRAWABLE_H
