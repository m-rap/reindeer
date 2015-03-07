#include "Light.h"
#include "OpenGLWorld.h"

Light::Light(void) : Camera()
{
	float s = 0.25f;
	projection = glm::ortho<float>(-10 * s, 10 * s, -10 * s, 10 * s, 0, 20);
}

Light::~Light(void)
{
}

void Light::Init()
{
	InitShadowMap();
}

RDRMAT4 Light::GetDepthMVP(const glm::mat4& world)
{
	return projection * view * world;
}

RDRMAT4 Light::GetDepthBiasMVP(const glm::mat4& world)
{
#ifdef USE_OPENGL
	static glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
    return biasMatrix * GetDepthMVP(world);
#endif
}