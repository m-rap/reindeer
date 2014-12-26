#include "OpenGLRenderer.h"
#include "ModelObject.h"
#include "objloader.h"
#include "texture.h"
#include "OpenGLWorld.h"

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

	SetProgramId(((OpenGLWorld*)World::Global)->standardShader);
	SetDepthShader(((OpenGLWorld*)World::Global)->depthShader);
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
	this->worldId = glGetUniformLocation(programId, "M");
	this->viewId = glGetUniformLocation(programId, "V");
	this->viewInvId = glGetUniformLocation(programId, "V_inv");
	this->normalMatId = glGetUniformLocation(programId, "N");
	this->textureId = glGetUniformLocation(programId, "texture");
	this->useTextureId = glGetUniformLocation(programId, "useTexture");
	this->depthBiasId = glGetUniformLocation(programId, "DepthBiasMVP");
	this->shadowMapId = glGetUniformLocation(programId, "shadowMap");
}

void OpenGLRenderer::SetDepthShader(const GLuint& id)
{
	this->depthRTTShader = id;
	this->depthMVPId = glGetUniformLocation(depthRTTShader, "depthMVP");
	this->depthVertexPositionId = glGetAttribLocation(depthRTTShader, "vertexPosition_modelspace");
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

void OpenGLRenderer::Draw(Camera* camera, Light* light)
{
	glm::vec3& lightPosition = *light->GetPosition();
	glm::mat4& projection = *camera->GetProjection();
	glm::mat4& view = *camera->GetView();
	glm::mat4& world = *parent->GetWorld();
	glm::mat4 modelview = view * world;
	glm::mat4 mvp = projection * modelview;
	glm::mat4 viewInv = glm::inverse(view);
	glm::mat3 normalMat = glm::inverseTranspose(glm::mat3(world));

	glm::mat4 depthMVP = light->GetDepthMVP(world);
	glm::mat4 depthBiasMat = light->GetDepthBiasMVP(depthMVP);

	glUniform3f(lightPositionId, lightPosition.x, lightPosition.y, lightPosition.z);
	glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(worldId, 1, GL_FALSE, &world[0][0]);
	//glUniformMatrix4fv(viewId, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(viewInvId, 1, GL_FALSE, &viewInv[0][0]);
	glUniformMatrix3fv(normalMatId, 1, GL_FALSE, &normalMat[0][0]);
	glUniformMatrix4fv(depthBiasId, 1, GL_FALSE, &depthBiasMat[0][0]);

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

	glUniform1i(useTextureId, useTexture ? 1 : 0);
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
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, light->GetDepthTexture());
	glUniform1i(shadowMapId, 1);

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
	if (useTexture)
		glDisableVertexAttribArray(uvId);
}

void OpenGLRenderer::RenderShadow(Light* light)
{
	glm::mat4& world = *parent->GetWorld();
	glm::mat4 depthMVP = light->GetDepthMVP(world);

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
	glUniformMatrix4fv(depthMVPId, 1, GL_FALSE, &depthMVP[0][0]);

	glEnableVertexAttribArray(depthVertexPositionId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
		depthVertexPositionId,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,//sizeof(GL_FLOAT) * 3,
		(void*)0
	);

	if (isIndexed)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	glDisableVertexAttribArray(depthVertexPositionId);
}
