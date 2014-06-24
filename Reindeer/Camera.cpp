#include "Camera.h"

Camera::Camera()
	: BaseObject()
{
	BuildProjection();
	BuildView();
}

Camera::~Camera()
{
}

void Camera::BuildProjection()
{
	D3DXMatrixPerspectiveFovLH(&projection,
        D3DXToRadian(45),    // the horizontal field of view
        (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
        1.0f,    // the near view-plane
        100.0f);    // the far view-plane
}

void Camera::BuildView()
{
	D3DXVECTOR4 v4;
	D3DXVECTOR3 up, lookAt(0, 0, 10);
	
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
}

D3DMATRIX Camera::GetView()
{
	return view;
}

D3DMATRIX Camera::GetProjection()
{
	return projection;
}

void Camera::BuildWorld()
{
	BaseObject::BuildWorld();
	BuildView();
}