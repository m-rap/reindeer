#include "LegacyOpenGLBoxRenderer.h"
#include "BoxObject.h"

LegacyOpenGLBoxRenderer::LegacyOpenGLBoxRenderer(BaseObject* parent) : BoxRenderer(parent)
{
}

LegacyOpenGLBoxRenderer::~LegacyOpenGLBoxRenderer()
{
}

void LegacyOpenGLBoxRenderer::SetProgramId(const GLuint& programId)
{
}

void LegacyOpenGLBoxRenderer::BuildBuffers()
{
	BoxObject* parent = (BoxObject*)this->parent;

	float width  = parent->GetWidth(),
		  height = parent->GetHeight(),
		  length = parent->GetLength();

	float vertices[] = {
		-width / 2, -height / 2,  length / 2,    // side 1
		 width / 2, -height / 2,  length / 2,
		-width / 2,  height / 2,  length / 2,
		 width / 2,  height / 2,  length / 2,

		-width / 2, -height / 2, -length / 2,    // side 2
		-width / 2,  height / 2, -length / 2,
		 width / 2, -height / 2, -length / 2,
		 width / 2,  height / 2, -length / 2,

		-width / 2,  height / 2, -length / 2,    // side 3
		-width / 2,  height / 2,  length / 2,
		 width / 2,  height / 2, -length / 2,
		 width / 2,  height / 2,  length / 2,

		-width / 2, -height / 2, -length / 2,    // side 4
		 width / 2, -height / 2, -length / 2,
		-width / 2, -height / 2,  length / 2,
		 width / 2, -height / 2,  length / 2,

		 width / 2, -height / 2, -length / 2,    // side 5
		 width / 2,  height / 2, -length / 2,
		 width / 2, -height / 2,  length / 2,
		 width / 2,  height / 2,  length / 2,

		-width / 2, -height / 2, -length / 2,    // side 6
		-width / 2, -height / 2,  length / 2,
		-width / 2,  height / 2, -length / 2,
		-width / 2,  height / 2,  length / 2,
	};
	memcpy(this->vertices, vertices, sizeof(vertices));
}

void LegacyOpenGLBoxRenderer::BuildNormalsIndices()
{
	GLfloat normals[] = {
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,
		 0.0f,  0.0f,  1.0f,

		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,
		 0.0f,  0.0f, -1.0f,

		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,

		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,
		 0.0f, -1.0f,  0.0f,

		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,
		 1.0f,  0.0f,  0.0f,

		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
		-1.0f,  0.0f,  0.0f,
	};
	memcpy(this->normals, normals, sizeof(normals));

	GLbyte indices[] = {
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};
	memcpy(this->indices, indices, sizeof(indices));
}

void LegacyOpenGLBoxRenderer::Draw(Camera* camera)
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
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);

	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_NORMAL_ARRAY);
}
