#pragma once

#ifndef LEGACYOPENGLLIGHT_H
#define LEGACYOPENGLLIGHT_H

#include "Light.h"
class LegacyOpenGLLight :
	public Light
{
protected:
	GLuint depthTexture;

	virtual bool InitShadowMap();

public:
	LegacyOpenGLLight();
	virtual ~LegacyOpenGLLight();

	virtual void SetDepthShader(const GLuint& id);
	virtual void SetStandardShader(const GLuint& id);
	virtual void SetTextureViewerShader(const GLuint& id);
	virtual GLuint GetDepthTexture();
	virtual GLuint GetDepthFrameBuffer();

	virtual void Init();

	virtual void DrawShadowMapTexture();
	virtual void ApplyShadowMap(Camera& camera);
	virtual void RenderLight();
	virtual void RenderDimLight();
};

#endif