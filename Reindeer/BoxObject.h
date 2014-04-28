#pragma once

#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include "Camera.h"

class BoxObject
	: public BaseObject
{
protected:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPDIRECT3DINDEXBUFFER9 indexBuffer;

	LPDIRECT3DDEVICE9 graphicsDevice;

	D3DVECTOR min, max;
	float width, height, length;

	void BuildWidthHeightLength();
	void BuildBuffers();
public:
	BoxObject(LPDIRECT3DDEVICE9 graphicsDevice = NULL);
	~BoxObject();

	void SetGraphicsDevice(const LPDIRECT3DDEVICE9& graphicsDevice);
	LPDIRECT3DDEVICE9 GetGraphicsDevice();

	void SetMin(const D3DVECTOR& value);
	D3DVECTOR GetMin();

	void SetMax(const D3DVECTOR& value);
	D3DVECTOR GetMax();

	void SetMinMax(const D3DVECTOR& min, const D3DVECTOR& max);

	void Draw(Camera& camera);
};

#endif