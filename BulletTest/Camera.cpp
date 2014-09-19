#include "Camera.h"

Camera::Camera() : BaseObject() {
	BuildProjection();
	BuildView();
}

Camera::~Camera() {
}

void Camera::BuildProjection() {
	projection = glm::perspective(
		(float)ToRadian(45),    // the horizontal field of view
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, // aspect ratio
		1.0f,    // the near view-plane
		100.0f);    // the far view-plane
}
void Camera::BuildView() {
	glm::vec4 up, lookAt(0, 0, 10, 0);
	lookAt = rotationMatrix * lookAt;
	lookAt = glm::vec4(position, 0) + lookAt;
	up = rotationMatrix * VECTOR_UP;

	view = glm::lookAt(position, glm::vec3(lookAt), glm::vec3(up));
}

glm::mat4* Camera::GetView() {
	return &view;
}

glm::mat4* Camera::GetProjection() {
	return &projection;
}

void Camera::BuildWorld() {
    BaseObject::BuildWorld();
    BuildView();
}
