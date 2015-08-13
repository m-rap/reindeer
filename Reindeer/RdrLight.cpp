#include "RdrLight.h"
#include "RdrWorld_OpenGL.h"

RdrLight::RdrLight(void) : RdrCamera()
{
}

RdrLight::~RdrLight(void)
{
}

void RdrLight::Init()
{
	InitShadowMap();
}

RDRMAT4 RdrLight::GetDepthMVP(const RDRMAT4& world)
{
	RDRMAT4 mvp;
	RdrHelper::Mat4Multiply(mvp, projection, view, world);
	return mvp;
}

RDRMAT4 RdrLight::GetDepthBiasMVP(const RDRMAT4& world)
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
