#pragma once
#ifndef BASERENDERER_H
#define BASERENDERER_H

#include "Camera.h"

class BaseRenderer
{
protected:
	BaseObject* parent;

public:
	BaseRenderer(BaseObject* parent);
	virtual ~BaseRenderer(void);

#ifdef USE_OPENGL
	virtual void SetProgramId(const GLuint& programId) = 0;
#endif
	virtual void BuildBuffers() = 0;
	virtual void Draw(Camera* camera) = 0;
};

#endif