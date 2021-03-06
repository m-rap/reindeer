#include "RdrMesh_LegacyOpenGL.h"
#include "texture.h"
#include "reindeer.h"

RdrMesh_LegacyOpenGL::RdrMesh_LegacyOpenGL(bool isIndexed, bool useTexture) : RdrMesh(isIndexed, useTexture)
{
	vertices = NULL;
	normals = NULL;
	indices = NULL;
	uvs = NULL;

	if (useTexture)
	{
		texture = loadDDS("../uvmap.DDS");
	}
}

RdrMesh_LegacyOpenGL::~RdrMesh_LegacyOpenGL(void)
{
	if (vertices != NULL)
		delete[] vertices;
	if (normals != NULL)
		delete[] normals;
	if (indices != NULL)
		delete[] indices;
	if (uvs != NULL)
		delete[] uvs;

	if (useTexture)
	{
		glDeleteTextures(1, &texture);
	}
}

void RdrMesh_LegacyOpenGL::BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	)
{
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;
	this->uvCount = uvCount;

	if (this->vertices != NULL)
		delete[] this->vertices;
	this->vertices = new GLfloat[vertexCount * 3];
	memcpy(this->vertices, &vertices[0].x, vertexCount * 3 * sizeof(GLfloat));

	if (this->normals != NULL)
		delete[] this->normals;
	this->normals = new GLfloat[vertexCount * 3];
	memcpy(this->normals, &normals[0].x, vertexCount * 3 * sizeof(GLfloat));

	if (isIndexed && indexCount > 0)
	{
		if (this->indices != NULL)
			delete[] this->indices;
		this->indices = new GLushort[indexCount];
		memcpy(this->indices, indices, indexCount * sizeof(unsigned short));
	}

	if (useTexture && uvCount > 0)
	{
		if (this->uvs != NULL)
			delete[] this->uvs;
		this->uvs = new GLfloat[uvCount * 2];
		memcpy(this->uvs, &uvs[0].x, uvCount * 2 * sizeof(GLfloat));
	}
}

void RdrMesh_LegacyOpenGL::RenderShadow(RdrTransform* tr, RdrLight* light)
{
	glm::mat4& projection = *light->GetProjection();
	glm::mat4& view = *light->GetView();
	glm::mat4& world = *tr->GetWorld();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glm::mat4 modelview = view * world;
	glLoadMatrixf(glm::value_ptr(modelview));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);

	if (isIndexed)
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_NORMAL_ARRAY);
}

void RdrMesh_LegacyOpenGL::Draw(RdrTransform* tr, RdrCamera* camera, RdrLight* light)
{
	glm::mat4& world = *tr->GetWorld();
	glm::mat4& projection = *camera->GetProjection();
	glm::mat4& view = *camera->GetView();

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projection));

	glMatrixMode(GL_MODELVIEW);
	glm::mat4 modelview = view * world;
	glLoadMatrixf(glm::value_ptr(modelview));

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);
	if (useTexture)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glActiveTexture(GL_TEXTURE0);glEnable(GL_TEXTURE_2D); // enables texture 0
        glBindTexture(GL_TEXTURE_2D, texture);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	}
	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //diffuse and ambient
	}

	glEnable(GL_COLOR_MATERIAL);

//#if !defined __arm__ && !defined __aarch64__
#ifndef USE_GLES
	//glColorMaterial(GL_FRONT, GL_AMBIENT); // the diffuse and ambient of material has been represented on color or texture
	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
#endif

	//float matAmbientAndDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbientAndDiffuse); // the diffuse and ambient of material has been represented on color or texture

	float matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);

	float matEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);

	glMaterialf(GL_FRONT, GL_SHININESS, 128);

	if (isIndexed)
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_NORMAL_ARRAY);

	if (useTexture)
	{
		glDisable(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D); // disables texture 0
	}
}
