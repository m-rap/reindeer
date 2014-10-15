#pragma once

#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "BaseObject.h"
#include "OpenGLModelRenderer.h"

class ModelObject :
	public BaseObject
{
private:
	ModelRenderer* renderer;
public:
	ModelObject(const char* modelPath);
	virtual ~ModelObject(void);

	std::vector<RDRVEC3> vertices;
	std::vector<RDRVEC2> uvs;
	std::vector<RDRVEC3> normals;
	
#ifdef USE_OPENGL
	void SetProgramId(GLuint programId);
#endif
	void LoadModel(const char* modelPath);
	void Draw(Camera* camera);
};

#endif