#pragma once
#ifndef D3D9MODELRENDERER_H
#define D3D9MODELRENDERER_H

#include "BaseRenderer.h"

class D3d9ModelRenderer :
	public BaseRenderer
{
private:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	size_t vertexCount;

public:
	D3d9ModelRenderer(BaseObject* parent);
	virtual ~D3d9ModelRenderer(void);

	virtual void BuildBuffers();
	virtual void Draw(Camera* camera);
};

#endif