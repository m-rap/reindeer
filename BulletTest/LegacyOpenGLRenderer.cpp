#include "LegacyOpenGLRenderer.h"
#include "ModelObject.h"

LegacyOpenGLRenderer::LegacyOpenGLRenderer(BaseObject* parent, bool isIndexed) : BaseRenderer(parent, isIndexed)
{
	vertices = NULL;
	normals = NULL;
	indices = NULL;
}

LegacyOpenGLRenderer::~LegacyOpenGLRenderer(void)
{
	if (vertices != NULL)
		delete[] vertices;
	if (normals != NULL)
		delete[] normals;
	if (indices != NULL)
		delete[] indices;
}

void LegacyOpenGLRenderer::BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	)
{
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;

	if (this->vertices != NULL)
		delete[] this->vertices;
	this->vertices = new GLfloat[vertexCount * 3];
	memcpy(this->vertices, &vertices[0].x, vertexCount * 3 * sizeof(GLfloat));

	if (this->normals != NULL)
		delete[] this->normals;
	this->normals = new GLfloat[vertexCount * 3];
	memcpy(this->normals, &normals[0].x, vertexCount * 3 * sizeof(GLfloat));

	if (indexCount > 0)
	{
		if (this->indices != NULL)
			delete[] this->indices;
		this->indices = new GLushort[indexCount];
		memcpy(this->indices, indices, indexCount * sizeof(unsigned short));
	}
}

void LegacyOpenGLRenderer::Draw(Camera* camera)
{
	glm::mat4& projection = *camera->GetProjection();
	glm::mat4& view = *camera->GetView();
	glm::mat4& world = *parent->GetWorld();

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
		glDrawElements(GL_TRIANGLES, indexCount, GL_SHORT, indices);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_NORMAL_ARRAY);
}