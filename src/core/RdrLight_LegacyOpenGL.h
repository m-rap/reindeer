#pragma once

#ifndef RDRLIGHT_LEGACYOPENGL_H
#define RDRLIGHT_LEGACYOPENGL_H

#include "RdrLight.h"

class RdrLight_LegacyOpenGL :
	public RdrLight
{
protected:
	GLuint depthTexture;

	virtual bool InitShadowMap();

public:
	RdrLight_LegacyOpenGL();
	virtual ~RdrLight_LegacyOpenGL();

	virtual void SetDepthShader(const GLuint& id);
	virtual void SetStandardShader(const GLuint& id);
	virtual void SetTextureViewerShader(const GLuint& id);
	virtual GLuint GetDepthTexture();
	virtual GLuint GetDepthFrameBuffer();

	virtual void Init();

	virtual void DrawShadowMapTexture();
	virtual void ApplyShadowMap(RdrCamera& camera);
	virtual void RenderLight();
	virtual void RenderDimLight();
};

#endif
