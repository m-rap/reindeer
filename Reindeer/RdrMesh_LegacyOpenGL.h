#pragma once
#ifndef LEGACYOPENGLRENDERER_H
#define LEGACYOPENGLRENDERER_H

#include "RdrCamera.h"
#include "RdrLight.h"

class RdrMesh_LegacyOpenGL :
	public RdrMesh
{
protected:
	GLfloat* vertices;
	GLfloat* normals;
	GLushort* indices;
	GLfloat* uvs;

	GLuint texture;

public:
	RdrMesh_LegacyOpenGL(bool isIndexed = false, bool useTexture = false);
	virtual ~RdrMesh_LegacyOpenGL(void);

	virtual void BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	);
	virtual void RenderShadow(RdrTransform* tr, RdrLight* light);
	virtual void Draw(RdrTransform* tr, RdrCamera* camera, RdrLight* light);
};

#endif
