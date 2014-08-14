#include "BoxObject.h"

BoxObject::BoxObject() : BaseObject() {
	min.x = min.y = min.z = 0;
	max.x = max.y = max.z = 0;

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenBuffers(1, &normalBuffer);

	BuildNormalsIndices();
	BuildWidthHeightLength();
}

BoxObject::~BoxObject() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &normalBuffer);
}

void BoxObject::SetMin(const glm::vec3& value) {
	this->min = value;
	BuildWidthHeightLength();
}

glm::vec3 BoxObject::GetMin() {
	return min;
}

void BoxObject::SetMax(const glm::vec3& value) {
	this->max = value;
	BuildWidthHeightLength();	
}


glm::vec3 BoxObject::GetMax() {
	return max;
}

void BoxObject::SetMinMax(const glm::vec3& min, const glm::vec3& max) {
	this->min = min;
	this->max = max;
	BuildWidthHeightLength();
}

void BoxObject::SetProgramId(const GLuint& programId) {
	this->programId = programId;
	this->positionId = glGetAttribLocation(programId, "vertexPosition_modelspace");
	this->normalId = glGetAttribLocation(programId, "vertexNormal_modelspace");
	this->mvpId = glGetUniformLocation(programId, "MVP");
	this->matrixId = glGetUniformLocation(programId, "M");
	this->viewId = glGetUniformLocation(programId, "V");
	this->viewInvId = glGetUniformLocation(programId, "V_inv");
	this->normalMatId = glGetUniformLocation(programId, "N");

}

void BoxObject::Draw(Camera& camera) {
	glm::mat4 projection = camera.GetProjection();
	glm::mat4 view = camera.GetView();
	glm::mat4 modelview = view * world;
	glm::mat4 mvp = projection * modelview;
	glm::mat4 viewInv = glm::inverse(view);
	glm::mat3 normalMat = glm::inverseTranspose(glm::mat3(world));
	
	//printf("normalMat\n%f %f %f\n%f %f %f\n%f %f %f", normalMat[0][0], normalMat[0][1], normalMat[0][2],
	//                                                  normalMat[1][0], normalMat[1][1], normalMat[1][2],
	//												  normalMat[2][0], normalMat[2][1], normalMat[2][2]);

	glUniformMatrix4fv(mvpId, 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &world[0][0]);
	glUniformMatrix4fv(viewId, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(viewInvId, 1, GL_FALSE, &viewInv[0][0]);
	glUniformMatrix3fv(normalMatId, 1, GL_FALSE, &normalMat[0][0]);

//	glm::vec3 na = glm::vec3(1.0), nb = glm::vec3(-1.0);
//	glm::vec3 nca = normalMat * na, ncb = normalMat * nb;
//	glm::vec4 l(0.0f, 5.0f, 0.0f, 1), a(min, 1), b(max, 1);
//	glm:: vec4 aa = glm::normalize(l - (world * a)), bb = glm::normalize(l - (world * b));
//	if (glm::dot(nca, glm::vec3(aa)) < 0.1f && glm::dot(ncb, glm::vec3(bb)) < 0.1f)
//	printf("a %f %f %f = %f\nb %f %f %f = %f\n", nca.x * aa.x, nca.y * aa.y, nca.z * aa.z, glm::dot(nca, glm::vec3(aa)),
//	                                             ncb.x * bb.x, ncb.y * bb.y, ncb.z * bb.z, glm::dot(ncb, glm::vec3(bb)));
//	printf("a %f %f %f = %f\nb %f %f %f = %f\n", nca.x, nca.y, nca.z, glm::dot(nca, glm::vec3(aa)),
//	                                             ncb.x, ncb.y, ncb.z, glm::dot(ncb, glm::vec3(bb)));
//	printf("a %f %f %f = %f\nb %f %f %f = %f\n", aa.x, aa.y, aa.z, glm::dot(nca, glm::vec3(aa)),
//	                                             bb.x, bb.y, bb.z, glm::dot(ncb, glm::vec3(bb)));

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

void BoxObject::BuildWidthHeightLength() {
	width  = max.x - min.x;
	height = max.y - min.y;
	length = max.z - min.z;

	BuildBuffers();
}

void BoxObject::BuildBuffers() {
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

void BoxObject::BuildNormalsIndices() {
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
