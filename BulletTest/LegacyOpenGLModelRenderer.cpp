#include "LegacyOpenGLModelRenderer.h"
#include "ModelObject.h"

LegacyOpenGLModelRenderer::LegacyOpenGLModelRenderer(BaseObject* parent) : BaseRenderer(parent)
{
	vertices = NULL;
	normals = NULL;
	indices = NULL;
}

LegacyOpenGLModelRenderer::~LegacyOpenGLModelRenderer(void)
{
	if (vertices != NULL)
		delete[] vertices;
	if (normals != NULL)
		delete[] normals;
	if (indices != NULL)
		delete[] indices;
}

void LegacyOpenGLModelRenderer::BuildBuffers()
{
	ModelObject* parent = (ModelObject*)this->parent;

	vertexCount = parent->vertices.size();

	if (vertices != NULL)
		delete[] vertices;
	vertices = new GLfloat[vertexCount * 3];
	memcpy(vertices, &parent->vertices[0].x, vertexCount * 3 * sizeof(GLfloat));

	if (normals != NULL)
		delete[] normals;
	normals = new GLfloat[vertexCount * 3];
	memcpy(normals, &parent->normals[0].x, vertexCount * 3 * sizeof(GLfloat));
}

void LegacyOpenGLModelRenderer::Draw(Camera* camera)
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
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_NORMAL_ARRAY);
}