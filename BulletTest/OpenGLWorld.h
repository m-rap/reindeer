#pragma once

#ifndef OPENGLWORLD_H
#define OPENGLWORLD_H

#include "World.h"

class OpenGLWorld :
	public World
{
private:
    GLuint lightPositionId;
    float lightPosition[4];

protected:
	virtual void Init3d();
	virtual void PreUpdate();
	virtual void PostUpdate();
	virtual void PostRender();

public:
	OpenGLWorld(void);
	virtual ~OpenGLWorld(void);
};

#endif
