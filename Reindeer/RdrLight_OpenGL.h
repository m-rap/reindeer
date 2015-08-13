#pragma once

#ifndef OPENGLLIGHT_H
#define OPENGLLIGHT_H

#include "RdrLight.h"
class RdrLight_OpenGL :
	public RdrLight
{
protected:
	GLuint depthShader;
	GLuint standardShader;
	GLuint textureViewerShader;

	GLuint depthMVPId;
	GLuint lightPositionId;

	GLuint textureViewerVertexId;
	GLuint textureViewerTextureId;

	GLuint depthTexture;
	GLuint depthFrameBuffer;
	GLuint quadVertexBuffer;

	virtual bool InitShadowMap();

public:
	RdrLight_OpenGL();
	virtual ~RdrLight_OpenGL();

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
