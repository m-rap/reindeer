#include "Camera.h"

Camera::Camera() : BaseObject() {
	BuildProjection();
	BuildView();
}

Camera::~Camera() {
}

void Camera::BuildProjection() {
#ifdef USE_D3D9
	D3DXMatrixPerspectiveFovRH(&projection,
        D3DXToRadian(45),    // the horizontal field of view
        (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
        1.0f,    // the near view-plane
        100.0f);    // the far view-plane
#else
	projection = glm::perspective(
		(float)ToRadian(45),    // the horizontal field of view
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, // aspect ratio
		1.0f,    // the near view-plane
		100.0f);    // the far view-plane
#endif
}
void Camera::BuildView() {
#ifdef USE_D3D9
	D3DXVECTOR4 v4;
	D3DXVECTOR3 up, lookAt(0, 0, -10);
	
	D3DXVec3Transform(&v4, &lookAt, &rotationMatrix);
	RdrHelper::Vec4ToVec3(lookAt, v4);
	D3DXVec3Add(&lookAt, &position, &lookAt);

	D3DXVec3Transform(&v4, &VECTOR_UP, &rotationMatrix);
	RdrHelper::Vec4ToVec3(up, v4);

	D3DXMatrixLookAtLH(
		&view,
		&position,    // the camera position
		&lookAt,    // the look-at position
		&up     // the up direction
	);
#else
	glm::vec4 up, lookAt(0, 0, 10, 0);
	lookAt = rotationMatrix * lookAt;
	lookAt = glm::vec4(position, 0) + lookAt;
	up = rotationMatrix * glm::vec4(VECTOR_UP, 0.0f);

	view = glm::lookAt(position, glm::vec3(lookAt), glm::vec3(up));
#endif
}

RDRMAT4* Camera::GetView() {
	return &view;
}

RDRMAT4* Camera::GetProjection() {
	return &projection;
}

void Camera::BuildWorld() {
    BaseObject::BuildWorld();
    BuildView();
}
