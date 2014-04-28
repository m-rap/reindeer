#include "RdrHelper.h"
#include <stdio.h>

RdrHelper::RdrHelper()
{
}


RdrHelper::~RdrHelper()
{
}

void RdrHelper::EulerToQuaternion(D3DXQUATERNION& q, D3DXVECTOR3& euler)
{
	D3DXQuaternionRotationYawPitchRoll(&q, euler.y, euler.x, euler.z);
	/* 
	 * command di bawah ini sama saja dengan D3DXQuaternionRotationYawPitchRoll
	 *

	D3DXQUATERNION tempQuaternion;
	D3DXQuaternionIdentity(&quaternion);

	D3DXQuaternionRotationAxis(&tempQuaternion, &VECTOR_BACKWARD, roll);
	D3DXQuaternionMultiply(&quaternion, &quaternion, &tempQuaternion);

	D3DXQuaternionRotationAxis(&tempQuaternion, &VECTOR_RIGHT, pitch);
	D3DXQuaternionMultiply(&quaternion, &quaternion, &tempQuaternion);
	
	D3DXQuaternionRotationAxis(&tempQuaternion, &VECTOR_UP, yaw);
	D3DXQuaternionMultiply(&quaternion, &quaternion, &tempQuaternion);
	 
	 */
}

void RdrHelper::QuaternionToEuler(D3DXVECTOR3& euler, D3DXQUATERNION& q)
{
	/*
	 * reference:
	 * http://xboxforums.create.msdn.com/forums/p/4574/23766.aspx#23766
	 * post by ed022 at 9/8/2007 1:54 AM
	 */
	
	D3DXMATRIX tempMat;
	D3DXMatrixRotationQuaternion(&tempMat, &q);

	D3DXVECTOR4 forward;
	D3DXVECTOR3 forward3;
	D3DXVec3Transform(&forward, &VECTOR_FORWARD, &tempMat);
	Vec4ToVec3(forward3, forward);

    D3DXVECTOR4 up;
	D3DXVECTOR3 up3;
	D3DXVec3Transform(&up, &VECTOR_UP, &tempMat);
	Vec4ToVec3(up3, up);

	D3DXVECTOR3 zeroV(0, 0, 0);
	AngleTo(euler, zeroV, forward3);
    if (euler.x == PIOVER2)
    {
        euler.y = atan2(up.x, up.z);
        euler.z = 0;
    }
    else if (euler.x == -PIOVER2)
    {
        euler.y = atan2(-up.x, -up.z);
        euler.z = 0;
    }
    else
    {
		D3DXMatrixRotationY(&tempMat, -euler.y);
        D3DXVec3Transform(&up, &up3, &tempMat);
		Vec4ToVec3(up3, up);
		
		D3DXMatrixRotationX(&tempMat, -euler.x);
        D3DXVec3Transform(&up, &up3, &tempMat);
		Vec4ToVec3(up3, up);
        
		euler.z = atan2(-up.x, up.y);
    }
}

void RdrHelper::AngleTo(D3DXVECTOR3& angle, const D3DXVECTOR3& from, const D3DXVECTOR3& location)
{
    D3DXVECTOR3 v3;
	D3DXVec3Subtract(&v3, &location, &from);
	D3DXVec3Normalize(&v3, &v3);
    angle.x = asin(v3.y);
    angle.y = atan2(-v3.x, -v3.z);
}

void RdrHelper::Vec4ToVec3(D3DXVECTOR3& result, D3DXVECTOR4& v4)
{
	result.x = v4.x / v4.w; result.y = v4.y / v4.w; result.z = v4.z / v4.w;
}

void RdrHelper::Vec3ToRadian(D3DXVECTOR3& output, const D3DXVECTOR3& input)
{
	output.x = D3DXToRadian(input.x);
	output.y = D3DXToRadian(input.y);
	output.z = D3DXToRadian(input.z);
}

void RdrHelper::Vec3ToDegree(D3DXVECTOR3& output, const D3DXVECTOR3& input)
{
	output.x = D3DXToDegree(input.x);
	output.y = D3DXToDegree(input.y);
	output.z = D3DXToDegree(input.z);
}