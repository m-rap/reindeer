#include "OpenGLModelRenderer.h"
#include "ModelObject.h"
#include "objloader.h"

OpenGLModelRenderer::OpenGLModelRenderer(ModelObject* parent) : ModelRenderer(parent)
{
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &normalBuffer);
}

OpenGLModelRenderer::~OpenGLModelRenderer(void)
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
}

void OpenGLModelRenderer::SetProgramId(const GLuint& programId)
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

void OpenGLModelRenderer::BuildBuffers()
{
	std::vector<RDRVEC3>& vertices = parent->vertices;
	std::vector<RDRVEC2>& uvs = parent->uvs;
	std::vector<RDRVEC3>& normals = parent->normals;

	vertexCount = vertices.size();

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
}

void OpenGLModelRenderer::Draw(Camera* camera)
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

	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(normalId);
	glDisableVertexAttribArray(positionId);
}