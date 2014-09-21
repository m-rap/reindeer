#include "OpenGLBoxRenderer.h"
#include "BoxObject.h"

OpenGLBoxRenderer::OpenGLBoxRenderer(BoxObject* parent) :
	BoxRenderer(parent)
{
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenBuffers(1, &normalBuffer);
}

OpenGLBoxRenderer::~OpenGLBoxRenderer()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &normalBuffer);
}

void OpenGLBoxRenderer::SetProgramId(const GLuint& programId)
{
	this->programId = programId;
	this->positionId = glGetAttribLocation(programId, "vertexPosition_modelspace");
	this->normalId = glGetAttribLocation(programId, "vertexNormal_modelspace");
	this->mvpId = glGetUniformLocation(programId, "MVP");
	this->matrixId = glGetUniformLocation(programId, "M");
	this->viewId = glGetUniformLocation(programId, "V");
	this->viewInvId = glGetUniformLocation(programId, "V_inv");
	this->normalMatId = glGetUniformLocation(programId, "N");
}

void OpenGLBoxRenderer::BuildBuffers()
{
	float width  = parent->GetWidth(),
		  height = parent->GetHeight(),
		  length = parent->GetLength();

	GLfloat vertices[] = {
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
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
}

void OpenGLBoxRenderer::BuildNormalsIndices()
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
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals[0], GL_STATIC_DRAW);

	GLuint indices[] = {
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);
}

void OpenGLBoxRenderer::Draw(Camera* camera)
{
	glm::mat4& projection = *camera->GetProjection();
	glm::mat4& view = *camera->GetView();
	glm::mat4& world = *parent->GetWorld();
	glm::mat4 modelview = view * world;
	glm::mat4 mvp = projection * modelview;
	glm::mat4 viewInv = glm::inverse(view);
	glm::mat3 normalMat = glm::inverseTranspose(glm::mat3(world));

	glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &world[0][0]);
	glUniformMatrix4fv(viewId, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(viewInvId, 1, GL_FALSE, &viewInv[0][0]);
	glUniformMatrix3fv(normalMatId, 1, GL_FALSE, &normalMat[0][0]);

	glEnableVertexAttribArray(positionId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
		positionId,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,//sizeof(GL_FLOAT) * 3,
		(void*)0
	);

	glEnableVertexAttribArray(normalId);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glVertexAttribPointer(
		normalId,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,//sizeof(GL_FLOAT) * 3,
		(void*)0
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(normalId);
	glDisableVertexAttribArray(positionId);
}
