#pragma once
#include "Light.h"
class D3d9Light :
	public Light
{
protected:
	virtual bool InitShadowMap() { return false; }
public:
	D3d9Light();
	virtual ~D3d9Light();

	virtual void DrawShadowMapTexture() {}
	virtual void ApplyShadowMap(Camera& camera) {}
	virtual void RenderLight() {}
	virtual void RenderDimLight() {}
};

