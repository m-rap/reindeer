#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Camera.h"

class Light :
	public Camera
{
protected:
	virtual bool InitShadowMap() = 0;

public:
	Light(void);
	virtual ~Light(void);

#ifdef USE_OPENGL
	virtual void SetDepthShader(const GLuint& id) = 0;
	virtual void SetStandardShader(const GLuint& id) = 0;
	virtual void SetTextureViewerShader(const GLuint& id) = 0;
	virtual GLuint GetDepthTexture() = 0;
	virtual GLuint GetDepthFrameBuffer() = 0;
#endif
	virtual void Init();

	RDRMAT4 GetDepthMVP(const RDRMAT4& world);
	RDRMAT4 GetDepthBiasMVP(const RDRMAT4& depthMVP);

	virtual void DrawShadowMapTexture() = 0;
	virtual void ApplyShadowMap(Camera& camera) = 0;
	virtual void RenderLight() = 0;
	virtual void RenderDimLight() = 0;
};

#endif
