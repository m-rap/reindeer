#pragma once

#ifndef RDRHELPER_H
#define RDRHELPER_H

#include <d3d9.h>
#include <d3dx9math.h>

const float PIOVER2 = D3DX_PI / 2;

const D3DXVECTOR3 VECTOR_RIGHT    (1,  0,  0);
const D3DXVECTOR3 VECTOR_LEFT     (1,  0,  0);
const D3DXVECTOR3 VECTOR_UP       (0,  1,  0);
const D3DXVECTOR3 VECTOR_DOWN     (0, -1,  0);
const D3DXVECTOR3 VECTOR_BACKWARD (0,  0,  1);
const D3DXVECTOR3 VECTOR_FORWARD  (0,  0, -1);

class RdrHelper
{
public:
	RdrHelper();
	~RdrHelper();

	static void EulerToQuaternion(D3DXQUATERNION& quaternion, D3DXVECTOR3& euler);
	static void QuaternionToEuler(D3DXVECTOR3& euler, D3DXQUATERNION& quaternion);
	static void AngleTo(D3DXVECTOR3& result, const D3DXVECTOR3& from, const D3DXVECTOR3& location);
	static void Vec4ToVec3(D3DXVECTOR3& result, D3DXVECTOR4& v4);
	static void Vec3ToRadian(D3DXVECTOR3& output, const D3DXVECTOR3& input);
	static void Vec3ToDegree(D3DXVECTOR3& output, const D3DXVECTOR3& input);
};

#endif