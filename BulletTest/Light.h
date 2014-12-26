#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "Camera.h"

class Light :
	public Camera
{
private:
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
	RDRVEC3 lookAt;

public:
	Light(void);
	virtual ~Light(void);

	//virtual void BuildProjection(bool perspective = true);
	//virtual void BuildView();

#ifdef USE_OPENGL
	void SetDepthShader(const GLuint& id);
	void SetStandardShader(const GLuint& id);
	void SetTextureViewerShader(const GLuint& id);
	GLuint GetDepthTexture();
	GLuint GetDepthFrameBuffer();
#endif
	void InitShadow();
	void InitQuad();
	bool PreShadow();
	void BuildDepthMVP();
	virtual void SetPosition(const RDRVEC3& position, bool silent = false);
	void SetLookAt(const RDRVEC3& lookAt, bool silent = false);
	RDRMAT4 GetDepthMVP(const glm::mat4& world);
	RDRMAT4 GetDepthBiasMVP(const glm::mat4& depthMVP);
	void DrawShadowTexture();
};

#endif
