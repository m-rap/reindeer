#include "OpenGLRenderer.h"
#include "ModelObject.h"
#include "objloader.h"
#include "texture.h"

OpenGLRenderer::OpenGLRenderer(BaseObject* parent, bool isIndexed, bool useTexture) : BaseRenderer(parent, isIndexed, useTexture)
{
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &normalBuffer);
	if (isIndexed)
		glGenBuffers(1, &indexBuffer);
	if (useTexture)
	{
		glGenBuffers(1, &uvBuffer);
		texture = loadDDS("../uvmap.DDS");
	}
}

OpenGLRenderer::~OpenGLRenderer(void)
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	if (isIndexed)
		glDeleteBuffers(1, &indexBuffer);
	if (useTexture)
	{
		glDeleteBuffers(1, &uvBuffer);
		glDeleteTextures(1, &texture);
	}
}

void OpenGLRenderer::SetProgramId(const GLuint& programId)
{
	this->programId = programId;
	this->positionId = glGetAttribLocation(programId, "vertexPosition_modelspace");
	this->normalId = glGetAttribLocation(programId, "vertexNormal_modelspace");
	this->uvId = glGetAttribLocation(programId, "vertexUV");
	this->mvpId = glGetUniformLocation(programId, "MVP");
	this->matrixId = glGetUniformLocation(programId, "M");
	this->viewId = glGetUniformLocation(programId, "V");
	this->viewInvId = glGetUniformLocation(programId, "V_inv");
	this->normalMatId = glGetUniformLocation(programId, "N");
	this->textureId = glGetUniformLocation(programId, "texture");
}

void OpenGLRenderer::BuildBuffers(
		RDRVEC3* vertices, RDRVEC3* normals, unsigned short* indices, RDRVEC2* uvs,
		size_t vertexCount, size_t indexCount, size_t uvCount
	)
{
	this->vertexCount = vertexCount;
	this->indexCount = indexCount;
	this->uvCount = uvCount;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	if (isIndexed && indexCount > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	}

	if (useTexture && uvCount > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvCount * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	}
}

void OpenGLRenderer::Draw(Camera* camera)
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

	if (useTexture)
	{
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(textureId, 0);

		glEnableVertexAttribArray(uvId);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(
                uvId,                   // The attribute we want to configure
                2,                            // size : U+V => 2
                GL_FLOAT,                     // type
                GL_FALSE,                     // normalized?
                0,                            // stride
                (void*)0                      // array buffer offset
        );

	}

	if (isIndexed)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	glDisableVertexAttribArray(normalId);
	glDisableVertexAttribArray(positionId);
}