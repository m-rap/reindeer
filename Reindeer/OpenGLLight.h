#pragma once

#ifndef OPENGLLIGHT_H
#define OPENGLLIGHT_H

#include "Light.h"
class OpenGLLight :
	public Light
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
	OpenGLLight();
	virtual ~OpenGLLight();

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