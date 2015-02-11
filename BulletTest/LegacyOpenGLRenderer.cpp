#include "LegacyOpenGLRenderer.h"
#include "ModelObject.h"
#include "texture.h"

LegacyOpenGLRenderer::LegacyOpenGLRenderer(BaseObject* parent, bool isIndexed, bool useTexture) : BaseRenderer(parent, isIndexed, useTexture)
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

LegacyOpenGLRenderer::~LegacyOpenGLRenderer(void)
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

void LegacyOpenGLRenderer::BuildBuffers(
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

void LegacyOpenGLRenderer::RenderShadow(Light* light)
{
	glm::mat4& projection = *light->GetProjection();
	glm::mat4& view = *light->GetView();
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
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_NORMAL_ARRAY);
}

void LegacyOpenGLRenderer::Draw(Camera* camera, Light* light)
{
    static glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
    glm::mat4& lightProjection = *light->GetProjection();
	glm::mat4& lightView = *light->GetView();
    glm::mat4 textureMatrix = biasMatrix * lightProjection * lightView;

    //glEnable(GL_TEXTURE_GEN_S);
    //glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    //glTexGenfv(GL_S, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][0], textureMatrix[1][0], textureMatrix[2][0], textureMatrix[3][0])));// textureMatrix[0]));// Vector4f(textureMatrix[0], textureMatrix[4], textureMatrix[8], textureMatrix[12])/*textureMatrix.GetRow(0)*/);

    //glEnable(GL_TEXTURE_GEN_T);
    //glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    //glTexGenfv(GL_T, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][1], textureMatrix[1][1], textureMatrix[2][1], textureMatrix[3][1])));//textureMatrix[1]));// Vector4f(textureMatrix[1], textureMatrix[5], textureMatrix[9], textureMatrix[13])/*textureMatrix.GetRow(1)*/);

    //glEnable(GL_TEXTURE_GEN_R);
    //glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    //glTexGenfv(GL_R, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][2], textureMatrix[1][2], textureMatrix[2][2], textureMatrix[3][2])));//textureMatrix[2]));// Vector4f(textureMatrix[2], textureMatrix[6], textureMatrix[10], textureMatrix[14])/*textureMatrix.GetRow(2)*/);

    //glEnable(GL_TEXTURE_GEN_Q);
    //glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
    //glTexGenfv(GL_Q, GL_EYE_PLANE, glm::value_ptr(glm::vec4(textureMatrix[0][3], textureMatrix[1][3], textureMatrix[2][3], textureMatrix[3][3])));//textureMatrix[3]));// Vector4f(textureMatrix[3], textureMatrix[7], textureMatrix[11], textureMatrix[15])/*textureMatrix.GetRow(3)*/);

    glEnable(GL_TEXTURE_2D);
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, light->GetDepthTexture());

    //Enable shadow comparison
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    //Shadow comparison should be true (ie not in shadow) if r<=texture
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    //Shadow comparison should generate an INTENSITY result
    //glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);

    //glEnable(GL_ALPHA_TEST);
    //glAlphaFunc(GL_GEQUAL, 0.99f);



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
	if (useTexture)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
		glTexCoordPointer(2, GL_FLOAT, 0, uvs);
	}
	else
	{
		glColor3f(1.0f, 1.0f, 1.0f); //diffuse and ambient
	}

	float matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);

	float matEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);

	glMaterialf(GL_FRONT, GL_SHININESS, 128);

	glColorMaterial(GL_FRONT, GL_AMBIENT);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glColorMaterial(GL_FRONT, GL_SPECULAR);

	glEnable(GL_COLOR_MATERIAL);

	if (isIndexed)
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indices);
	else
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisable(GL_VERTEX_ARRAY);
	glDisable(GL_NORMAL_ARRAY);

	if (useTexture)
	{
		glDisable(GL_TEXTURE_COORD_ARRAY);
	}
	glDisable(GL_TEXTURE_2D);
	//glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);
    glDisable(GL_TEXTURE_GEN_Q);
}
