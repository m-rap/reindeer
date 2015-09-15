#pragma once
#ifndef RDRMESH_OPENGL_H
#define RDRMESH_OPENGL_H

#include "RdrMesh.h"
#include "RdrLight.h"

class RdrMesh_OpenGL :
	public RdrMesh
{
protected:
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint indexBuffer;
	GLuint uvBuffer;

	GLuint programId;
	GLuint depthRTTShader;
	GLuint shadowMappingShader;

	GLuint lightPositionId;
	GLuint positionId;
	GLuint normalId;
	GLuint mvpId;
	GLuint worldId;
	GLuint viewId;
	GLuint viewInvId;
	GLuint normalMatId;
	GLuint textureId;
	GLuint uvId;
	GLuint useTextureId;
	GLuint depthBiasId;

	GLuint depthVertexPositionId;
	GLuint shadowMapId;
	GLuint shadowMap2Id;
	GLuint depthMVPId;

	GLuint texture;
public:
	RdrMesh_OpenGL(bool isIndexed = false, bool useTexture = false);
	virtual ~RdrMesh_OpenGL(void);

	virtual void SetProgramId(const GLuint& programId);
	virtual void SetDepthShader(const GLuint& id);
	virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	);
	virtual void RenderShadow(RdrTransform* tr, RdrLight* light);
	virtual void Draw(RdrTransform* tr, RdrCamera* camera, RdrLight* light);
};

#endif
