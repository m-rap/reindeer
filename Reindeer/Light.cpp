#include "Light.h"
#include "OpenGLWorld.h"

Light::Light(void) : Camera()
{
}

Light::~Light(void)
{
}

void Light::Init()
{
	InitShadowMap();
}

RDRMAT4 Light::GetDepthMVP(const RDRMAT4& world)
{
	RDRMAT4 mvp;
	RdrHelper::Mat4Multiply(mvp, projection, view, world);
	return mvp;
}

RDRMAT4 Light::GetDepthBiasMVP(const RDRMAT4& world)
{
	static RDRMAT4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	RDRMAT4 mvp;
    RdrHelper::Mat4Multiply(mvp, biasMatrix, GetDepthMVP(world));
	return mvp;
}