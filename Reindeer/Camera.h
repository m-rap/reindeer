#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "BaseObject.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Camera
	: public BaseObject
{
protected:
	D3DXMATRIX view, projection;
public:
	Camera();
	~Camera();

	void BuildProjection();
	void BuildView();

	D3DMATRIX GetView();
	D3DMATRIX GetProjection();

	virtual void SetPosition(const D3DXVECTOR3& position);
	virtual void SetRotation(const D3DXVECTOR3& rotation);
};

#endif