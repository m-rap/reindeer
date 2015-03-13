#pragma once
#ifndef BASERENDERER_H
#define BASERENDERER_H

#include "Camera.h"
#include "Light.h"

class BaseRenderer
{
protected:
	BaseObject* parent;
	bool isIndexed;
	bool useTexture;
	size_t vertexCount;
	size_t indexCount;
	size_t uvCount;

public:
	BaseRenderer(BaseObject* parent, bool isIndexed = false, bool useTexture = false);
	virtual ~BaseRenderer(void);

	virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	) = 0;
	virtual void RenderShadow(Light* light) = 0;
	virtual void Draw(Camera* camera, Light* light) = 0;
};

#endif