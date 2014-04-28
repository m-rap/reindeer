#pragma once

#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include "RdrHelper.h"

struct CUSTOMVERTEX {FLOAT X, Y, Z; D3DVECTOR NORMAL;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)

class BaseObject
{
protected:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXVECTOR3 scale;
	D3DXQUATERNION quaternion;

	D3DXMATRIX rotationMatrix;
	D3DXMATRIX world;

	void EulerToQuaternion();
	void QuaternionToEuler();
public:
	BaseObject();
	~BaseObject();

	virtual void SetPosition(const D3DXVECTOR3& position, bool silent = false);
	virtual D3DXVECTOR3 GetPosition();

	virtual void SetRotation(const D3DXVECTOR3& rotation, bool silent = false);
	virtual D3DXVECTOR3 GetRotation();

	virtual void SetScale(const D3DXVECTOR3& scale, bool silent = false);
	virtual D3DXVECTOR3 GetScale();

	void BuildWorld();
	D3DXMATRIX GetWorld();
};

#endif