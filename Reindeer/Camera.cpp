#include "Camera.h"

Camera::Camera() : BaseObject()
{
	BuildProjection();
	BuildView();
}

Camera::~Camera()
{
}

void Camera::BuildProjection(bool perspective)
{
#ifdef USE_D3D9
	D3DXMatrixPerspectiveFovRH(&projection,
        D3DXToRadian(45),    // the horizontal field of view
        (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
        1.0f,    // the near view-plane
        100.0f);    // the far view-plane
#else
	if (perspective)
	{
		projection = glm::perspective(
			(float)ToRadian(45),    // the horizontal field of view
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, // aspect ratio
			1.0f,    // the near view-plane
			100.0f);    // the far view-plane
	}
	else
	{
		float s = 1.0f;
		projection = glm::ortho<float>(-10*s, 10*s, -10*s, 10*s, 0*s, 20*s);
	}
#endif
}
void Camera::BuildView() {
#ifdef USE_D3D9
	D3DXVECTOR3 up, lookAt(0, 0, 10);

	lookAt = RdrHelper::Vec3Transform(rotation, lookAt);
	D3DXVec3Add(&lookAt, &position, &lookAt);

	up = RdrHelper::Vec3Transform(rotation, VECTOR_UP);

	D3DXMatrixLookAtRH(
		&view,
		&position,    // the camera position
		&lookAt,    // the look-at position
		&up     // the up direction
	);
#else
	glm::vec3 lookAt, up;
	lookAt = rotation * VECTOR_FORWARD;
	lookAt = position + lookAt;
	up = rotation * VECTOR_UP;

	view = glm::lookAt(position, glm::vec3(lookAt), up);
#endif
}

RDRMAT4* Camera::GetView() {
	return &view;
}

RDRMAT4* Camera::GetProjection() {
	return &projection;
}

void Camera::BuildWorld() {
    BuildView();
	//rotation = glm::quat_cast(view);
	BaseObject::BuildWorld();
}
