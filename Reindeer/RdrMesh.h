#pragma once

#ifndef RDRMESH_H
#define RDRMESH_H

#include "RdrDrawable.h"

using namespace std;

class RdrMesh :
    public RdrDrawable
{
protected:
    bool isIndexed;
	bool useTexture;
	size_t vertexCount;
	size_t indexCount;
	size_t uvCount;

public:
	RdrMesh(bool isIndexed = false, bool useTexture = false);
	virtual ~RdrMesh();

    virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	) = 0;
};

#endif
