#include "RdrCamera.h"

RdrCamera::RdrCamera()
{
    BuildProjection();
    BuildView();
}

RdrCamera::~RdrCamera()
{
}

void RdrCamera::BuildProjection(bool perspective)
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
void RdrCamera::BuildView() {
#ifdef USE_D3D9
    D3DXVECTOR3 up, lookAt;

    lookAt = RdrHelper::Vec3Transform(rotation, VECTOR_FORWARD);
    D3DXVec3Add(&lookAt, &position, &lookAt);

    up = RdrHelper::Vec3Transform(rotation, VECTOR_UP);

    D3DXMatrixLookAtRH(
        &view,
        &position,    // the camera position
        &lookAt,    // the look-at position
        &up     // the up direction
    );
    //view._31 *= -1;
    //view._32 *= -1;
    //view._33 *= -1;
    //view._34 *= -1;
#else
    glm::vec3 lookAt, up;
    RDRQUAT* tempRotation = tr.GetRotation();
    RDRVEC3* tempPosition = tr.GetPosition();
    lookAt = *tempRotation * VECTOR_FORWARD;
    lookAt = *tr.GetPosition() + lookAt;
    up = *tempRotation * VECTOR_UP;

    view = glm::lookAt(*tempPosition, glm::vec3(lookAt), up);
#endif
}

RDRMAT4* RdrCamera::GetView() {
    return &view;
}

RDRMAT4* RdrCamera::GetProjection() {
    return &projection;
}
