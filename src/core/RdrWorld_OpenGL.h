#pragma once

#ifndef OPENGLWORLD_H
#define OPENGLWORLD_H

#include "RdrWorld.h"

class RdrWorld_OpenGL :
	public RdrWorld
{
private:
    float lightPosition[4];
    void PreShadow();
    void PostShadow();
    void PreDraw();
    void PostDraw();

protected:
	virtual void Init3d(int argc, char *argv[]);
	virtual void PostRender();
	virtual void Draw();

public:
	RdrWorld_OpenGL(Container* container);
	virtual ~RdrWorld_OpenGL(void);

	GLuint standardShader;
	GLuint depthShader;
	GLuint textureViewerShader;
};

#endif
