#include "RdrTransform.h"
#include "glm/gtx/quaternion.hpp"

RdrTransform::RdrTransform() {
	position.x = position.y = position.z = 0.0f;
	scale.x = scale.y = scale.z = 1.0f;
#ifdef USE_OPENGL
	rotation = RDRQUAT(1.0f, 0.0f, 0.0f, 0.0f);
#else
	rotation = RDRQUAT(0.0f, 0.0f, 0.0f, 1.0f);
#endif

	BuildWorld();
}

RdrTransform::~RdrTransform() {
	// TODO Auto-generated destructor stub
}

void RdrTransform::SetPosition(const RDRVEC3& position, bool silent) {
	this->position = position;

	if (!silent)
		BuildWorld();

	TransformationChanged();
}

void RdrTransform::SetRotation(const RDRQUAT& rotation, bool silent) {
	this->rotation = rotation;

	if (!silent)
		BuildWorld();

	TransformationChanged();
}

void RdrTransform::Translate(const RDRVEC3& position, bool silent)
{
    RDRVEC3 worldAxis = RdrHelper::Vec3Transform(rotation, position);
    this->position.x += worldAxis.x;
    this->position.y += worldAxis.y;
    this->position.z += worldAxis.z;

    if (!silent)
		BuildWorld();

	TransformationChanged();
}

void RdrTransform::Rotate(const float& angle, const RDRVEC3& axis, bool silent)
{
	Rotate(RdrHelper::AngleAxis(angle, axis), silent);
}

void RdrTransform::Rotate(const RDRQUAT& rotation, bool silent)
{
    this->rotation = RdrHelper::QuatMultiply(rotation, this->rotation);

	if (!silent)
		BuildWorld();

	TransformationChanged();
}

void RdrTransform::RotateLocalX(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = RdrHelper::Vec3Transform(rotation, AXIS_X);
	Rotate(angle, worldAxis, silent);
}

void RdrTransform::RotateLocalY(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = RdrHelper::Vec3Transform(rotation, AXIS_Y);
	Rotate(angle, worldAxis, silent);
}

void RdrTransform::RotateLocalZ(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = RdrHelper::Vec3Transform(rotation, AXIS_Z);
	Rotate(angle, worldAxis, silent);
}

void RdrTransform::RotateGlobalX(const float& angle, bool silent)
{
	Rotate(angle, AXIS_X, silent);
}

void RdrTransform::RotateGlobalY(const float& angle, bool silent)
{
	Rotate(angle, AXIS_Y, silent);
}

void RdrTransform::RotateGlobalZ(const float& angle, bool silent)
{
	Rotate(angle, AXIS_Z, silent);
}

void RdrTransform::LookAt(const RDRVEC3& target, bool silent)
{
	SetRotation(RdrHelper::LookAt(position, target, RdrHelper::Vec3Transform(rotation, VECTOR_UP)), silent);
}

void RdrTransform::SetScale(const RDRVEC3& scale, bool silent) {
	this->scale = scale;
	if (!silent)
		BuildWorld();

	TransformationChanged();
}

RDRVEC3* RdrTransform::GetPosition() {
	return &position;
}

RDRQUAT* RdrTransform::GetRotation() {
	return &rotation;
}

RDRVEC3* RdrTransform::GetScale() {
	return &scale;
}

RDRMAT4* RdrTransform::GetWorld()
{
	return &world;
}

void RdrTransform::BuildWorld()
{
	RdrHelper::BuildWorld(world, position, rotation, scale);
}
