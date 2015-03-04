#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Camera.h"

class Light :
	public Camera
{
protected:
#ifdef USE_OPENGL
	GLuint depthShader;
	GLuint standardShader;
	GLuint textureViewerShader;

	GLuint depthMVPId;
	GLuint lightPositionId;

	GLuint textureViewerVertexId;
	GLuint textureViewerTextureId;

	GLuint depthFrameBuffer;
	GLuint depthTexture;
	GLuint quadVertexBuffer;
#endif

	bool InitShadowMap();

public:
	Light(void);
	virtual ~Light(void);

#ifdef USE_OPENGL
	void SetDepthShader(const GLuint& id);
	void SetStandardShader(const GLuint& id);
	void SetTextureViewerShader(const GLuint& id);
	GLuint GetDepthTexture();
	GLuint GetDepthFrameBuffer();
#endif
	void Init();

	RDRMAT4 GetDepthMVP(const glm::mat4& world);
	RDRMAT4 GetDepthBiasMVP(const glm::mat4& depthMVP);

	void DrawShadowMapTexture();
	void ApplyShadowMap(Camera& camera);
	void RenderLight();
	void RenderDimLight();
};

#endif
