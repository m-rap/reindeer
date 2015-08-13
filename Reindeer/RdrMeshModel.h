#pragma once
#ifndef RDRMESHMODEL_H
#define RDRMESHMODEL_H

#include "Camera.h"
#include "Light.h"

class RdrMesh;

class RdrMeshModel :
    public Drawable
{
protected:
	RdrMesh* parent;
	bool isIndexed;
	bool useTexture;
	size_t vertexCount;
	size_t indexCount;
	size_t uvCount;

public:
	RdrMeshModel(RdrMesh* parent, bool isIndexed = false, bool useTexture = false);
	virtual ~RdrMeshModel(void);

	virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	) = 0;
};

#endif
