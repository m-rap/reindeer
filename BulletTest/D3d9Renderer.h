#pragma once
#ifndef D3D9RENDERER_H
#define D3D9RENDERER_H

#include "BaseRenderer.h"

class D3d9Renderer :
	public BaseRenderer
{
protected:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;

public:
	D3d9Renderer(BaseObject* parent, bool isIndexed = false);
	virtual ~D3d9Renderer(void);

	virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	);
	virtual void Draw(Camera* camera);
};

#endif