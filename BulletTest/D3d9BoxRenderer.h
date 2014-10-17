#pragma once
#ifndef D3D9BOXRENDERER
#define D3D9BOXRENDERER

#include "BoxRenderer.h"

class D3d9BoxRenderer :
	public BoxRenderer
{
private:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;

public:
	D3d9BoxRenderer(BaseObject* parent);
	virtual ~D3d9BoxRenderer(void);

	virtual void BuildBuffers();
	virtual void BuildNormalsIndices();
	virtual void Draw(Camera* camera);
};

#endif