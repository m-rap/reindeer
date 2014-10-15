#pragma once
#ifndef MODELRENDERER_H
#define MODELRENDERER_H

#include "Camera.h"

class ModelObject;

class ModelRenderer
{
protected:
	ModelObject* parent;
public:
	ModelRenderer(ModelObject* parent);
	virtual ~ModelRenderer(void);

#ifdef USE_OPENGL
	virtual void SetProgramId(const GLuint& programId) = 0;
#endif
	virtual void BuildBuffers() = 0;
	virtual void Draw(Camera* camera) = 0;
};

#endif