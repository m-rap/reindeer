#include "BaseObject.h"

BaseObject::BaseObject() {
	position.x = position.y = position.z = 0.0f;
	euler.x = euler.y = euler.z = 0.0f;
	scale.x = scale.y = scale.z = 1.0f;

	RDRVEC3 rotRad;
	RdrHelper::Vec3ToRadian(rotRad, this->euler);
	RdrHelper::EulerToQuaternion(quaternion, rotRad);

	BuildWorld();
}

BaseObject::~BaseObject() {
	// TODO Auto-generated destructor stub
}

void BaseObject::SetPosition(const RDRVEC3& position, bool silent) {
	this->position = position;
	
	if (!silent)
		BuildWorld();
}

void BaseObject::SetEuler(const RDRVEC3& euler, bool silent) {
	this->euler.x = fmod(euler.x, 360.0f);
	this->euler.y = fmod(euler.y, 360.0f);
	this->euler.z = fmod(euler.z, 360.0f);

	RDRVEC3 rotRad;
	RdrHelper::Vec3ToRadian(rotRad, this->euler);
	RdrHelper::EulerToQuaternion(quaternion, rotRad);

	if (!silent) {
		BuildWorld();
	}
}

void BaseObject::SetScale(const RDRVEC3& scale, bool silent) {
	this->scale = scale;
	if (!silent)
		BuildWorld();
}

RDRVEC3* BaseObject::GetPosition() {
	return &position;
}

RDRVEC3* BaseObject::GetEuler() {
	return &euler;
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
	D3DXMatrixTranslation(&tempMatrix, position.x * -1, position.y, position.z);
	D3DXMatrixMultiply(&world, &world, &tempMatrix);

#elif defined( USE_OPENGL )
	rotationMatrix = glm::toMat4(quaternion);
	world = glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
#endif
}