#include "BaseObject.h"
#include "glm/gtx/quaternion.hpp"

BaseObject::BaseObject() {
	position.x = position.y = position.z = 0.0f;
	scale.x = scale.y = scale.z = 1.0f;

	RDRVEC3 euler(0.0f);
	RdrHelper::EulerDegreeToQuaternion(rotation, euler);

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
	rotation = rotation * glm::angleAxis(angle, axis);

	if (!silent)
		BuildWorld();

	TransformationChanged();
}

void BaseObject::RotateLocalX(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = rotation * AXIS_X;
	Rotate(angle, worldAxis, silent);
}

void BaseObject::RotateLocalY(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = rotation * AXIS_Y;
	Rotate(angle, worldAxis, silent);
}

void BaseObject::RotateLocalZ(const float& angle, bool silent)
{
	RDRVEC3 worldAxis = rotation * AXIS_Z;
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

void BaseObject::BuildWorld() {
#ifdef USE_D3D9
	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity(&world);

	// scale
	D3DXMatrixScaling(&tempMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&world, &world, &tempMatrix);

	// rotation
	D3DXMatrixRotationQuaternion(&rotationMatrix, &quaternion);
	D3DXMatrixMultiply(&world, &world, &rotationMatrix);

	// position
	D3DXMatrixTranslation(&tempMatrix, position.x, position.y, position.z);
	D3DXMatrixMultiply(&world, &world, &tempMatrix);

#elif defined( USE_OPENGL )
	rotationMatrix = glm::toMat4(rotation);
	world = glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
#endif
}