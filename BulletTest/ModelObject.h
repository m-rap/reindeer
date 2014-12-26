#pragma once

#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "PhysicalObject.h"
#include "Drawable.h"
#include "BaseRenderer.h"

class ModelObject :
	public PhysicalObject,
	public Drawable
{
private:
	BaseRenderer* renderer;

protected:
	char* modelPath;
	virtual void BuildShape();
	size_t vertexCount;

public:
	ModelObject(const char* modelPath);
	virtual ~ModelObject(void);

	std::vector<RDRVEC3> vertices;
	std::vector<RDRVEC2> uvs;
	std::vector<RDRVEC3> normals;

	void LoadModel(const char* modelPath);
	virtual void RenderShadow(Light* light);
	virtual void Draw(Camera* camera, Light* light);

};

#endif
