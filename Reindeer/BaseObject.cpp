#include "BaseObject.h"


BaseObject::BaseObject()
{
	position.x = position.y = position.z = 0.0f;
	euler.x = euler.y = euler.z = 0.0f;
	scale.x = scale.y = scale.z = 1.0f;

	EulerToQuaternion();

	BuildWorld();
}


BaseObject::~BaseObject()
{
}

void BaseObject::SetPosition(const D3DXVECTOR3& position, bool silent)
{
	this->position = position;
	
	if (!silent)
		BuildWorld();
}

D3DXVECTOR3 BaseObject::GetPosition()
{
	return position;
}

void BaseObject::SetEuler(const D3DXVECTOR3& euler, bool silent)
{
	this->euler.x = fmod(euler.x, 360);
	this->euler.y = fmod(euler.y, 360);
	this->euler.z = fmod(euler.z, 360);

	EulerToQuaternion();
	
	if (!silent)
		BuildWorld();
}

D3DXVECTOR3 BaseObject::GetEuler()
{
	return euler;
}

void BaseObject::SetScale(const D3DXVECTOR3& scale, bool silent)
{
	this->scale = scale;
	
	if (!silent)
		BuildWorld();
}

D3DXVECTOR3 BaseObject::GetScale()
{
	return scale;
}

D3DXMATRIX BaseObject::GetWorld()
{
	return world;
}

void BaseObject::EulerToQuaternion()
{
	D3DXVECTOR3 rotRad;
	RdrHelper::Vec3ToRadian(rotRad, this->euler);
	RdrHelper::EulerToQuaternion(quaternion, rotRad);
}

void BaseObject::QuaternionToEuler()
{
	//D3DXQuaternionToAxisAngle(quaternion, 
}

void BaseObject::BuildWorld()
{
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
}