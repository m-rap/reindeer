#pragma once

#ifndef OPENGLWORLD_H
#define OPENGLWORLD_H

#include "World.h"

class OpenGLWorld :
	public World
{
private:
    float lightPosition[4];

protected:
	virtual void Init3d();
	virtual void PreRender();
	virtual void PreUpdate();
	virtual void PostUpdate();
	virtual void PostRender();
	virtual void Draw();

public:
	OpenGLWorld(Container* container);
	virtual ~OpenGLWorld(void);

	GLuint standardShader;
	GLuint depthShader;
	GLuint textureViewerShader;
};

#endif
