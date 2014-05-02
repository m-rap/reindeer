/*
 * BoxObject.cpp
 *
 *  Created on: May 2, 2014
 *      Author: MRP
 */

#include "BoxObject.h"
#include <GL/glm/gtc/type_ptr.hpp>

BoxObject::BoxObject() {
	vertices[0] = glm::vec3(-3, -3, -3);
	vertices[1] = glm::vec3( 3, -3, -3);
	vertices[2] = glm::vec3( 3, -3,  3);
	vertices[3] = glm::vec3(-3, -3,  3);

	vertices[4] = glm::vec3(-3,  3, -3);
	vertices[5] = glm::vec3( 3,  3, -3);
	vertices[6] = glm::vec3( 3,  3,  3);
	vertices[7] = glm::vec3(-3,  3,  3);

	short indices[] =
	{
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

BoxObject::~BoxObject() {
}

void BoxObject::Draw(Camera& camera) {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(camera.GetProjection()));
	glMatrixMode(GL_VIEWPORT);
	glLoadMatrixf(glm::value_ptr(camera.GetView()));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(world));

	glColor3f(0.0f,0.0f,1.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 36;) {
		glVertex2fv(glm::value_ptr(vertices[indices[i++]]));
		glVertex2fv(glm::value_ptr(vertices[indices[i++]]));
		glVertex2fv(glm::value_ptr(vertices[indices[i++]]));
	}
	glEnd();
}
