#pragma once
#ifndef RDRLIGHT_H
#define RDRLIGHT_H

#include "RdrCamera.h"

class RdrLight :
	public RdrCamera
{
protected:
	virtual bool InitShadowMap() = 0;

public:
	RdrLight(void);
	virtual ~RdrLight(void);

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
	virtual void ApplyShadowMap(RdrCamera& camera) = 0;
	virtual void RenderLight() = 0;
	virtual void RenderDimLight() = 0;
};

#endif
