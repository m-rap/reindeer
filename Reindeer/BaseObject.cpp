#include "BaseObject.h"
#include "glm/gtx/quaternion.hpp"

BaseObject::BaseObject() {
	position.x = position.y = position.z = 0.0f;
	scale.x = scale.y = scale.z = 1.0f;
#ifdef USE_OPENGL
	rotation = RDRQUAT(1.0f, 0.0f, 0.0f, 0.0f);
#else
	rotation = RDRQUAT(0.0f, 0.0f, 0.0f, 1.0f);
#endif

	BuildWorld();
}

BaseObject::~BaseObject() {
	// TODO Auto-generated destructor stub
}

void BaseObject::SetPosition(const RDRVEC3& position, bool silent) {
	this->position = position;

	if (!silent)
		BuildWorld();

	TransformationChanged();
}

void BaseObject::SetRotation(const RDRQUAT& rotation, bool silent) {
	this->rotation = rotation;

	if (!silent)
		BuildWorld();

	TransformationChanged();
}

void BaseObject::Rotate(const float& angle, const RDRVEC3& axis, bool silent)
{
	rotation = RdrHelper::QuatMultiply(RdrHelper::AngleAxis(angle, axis), rotation);

	if (!silent)
		BuildWorld();

	TransformationChanged();
}

void BaseObject::RotateLocalX(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = RdrHelper::Vec3Transform(rotation, AXIS_X);
	Rotate(angle, worldAxis, silent);
}

void BaseObject::RotateLocalY(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = RdrHelper::Vec3Transform(rotation, AXIS_Y);
	Rotate(angle, worldAxis, silent);
}

void BaseObject::RotateLocalZ(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = RdrHelper::Vec3Transform(rotation, AXIS_Z);
	Rotate(angle, worldAxis, silent);
}

void BaseObject::RotateGlobalX(const float& angle, bool silent)
{
	Rotate(angle, AXIS_X, silent);
}

void BaseObject::RotateGlobalY(const float& angle, bool silent)
{
	Rotate(angle, AXIS_Y, silent);
}

void BaseObject::RotateGlobalZ(const float& angle, bool silent)
{
	Rotate(angle, AXIS_Z, silent);
}

void BaseObject::LookAt(const RDRVEC3& target, bool silent)
{
	SetRotation(RdrHelper::LookAt(position, target, RdrHelper::Vec3Transform(rotation, VECTOR_UP)), silent);
}

void BaseObject::SetScale(const RDRVEC3& scale, bool silent) {
	this->scale = scale;
	if (!silent)
		BuildWorld();

	TransformationChanged();
}

RDRVEC3* BaseObject::GetPosition() {
	return &position;
}

RDRQUAT* BaseObject::GetRotation() {
	return &rotation;
}

RDRVEC3* BaseObject::GetScale() {
	return &scale;
}

RDRMAT4* BaseObject::GetWorld()
{
	return &world;
}

void BaseObject::BuildWorld()
{
	RdrHelper::BuildWorld(world, position, rotation, scale);
}
