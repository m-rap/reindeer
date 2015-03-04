#include "reindeer.h"
#include "RdrHelper.h"

const RDRVEC3 VECTOR_UP       ( 0.0f,  1.0f,  0.0f);
const RDRVEC3 VECTOR_RIGHT    ( 1.0f,  0.0f,  0.0f);
const RDRVEC3 VECTOR_LEFT     (-1.0f,  0.0f,  0.0f);
const RDRVEC3 VECTOR_DOWN     ( 0.0f, -1.0f,  0.0f);
const RDRVEC3 VECTOR_BACKWARD ( 0.0f,  0.0f,  1.0f);
const RDRVEC3 VECTOR_FORWARD  ( 0.0f,  0.0f, -1.0f);

const RDRVEC3 AXIS_X(1.0f, 0.0f,  0.0f);
const RDRVEC3 AXIS_Y(0.0f, 1.0f,  0.0f);
const RDRVEC3 AXIS_Z(0.0f, 0.0f, -1.0f);

#ifdef USE_D3D9
const float PIOVER2 = D3DX_PI / 2;
#endif

RdrHelper::RdrHelper()
{
}


RdrHelper::~RdrHelper()
{
}

void RdrHelper::EulerDegreeToQuaternion(RDRQUAT& quaternion, const RDRVEC3& euler)
{
	RDRVEC3 tempEuler;
	tempEuler.x = fmod(euler.x, 360.0f);
	tempEuler.y = fmod(euler.y, 360.0f);
	tempEuler.z = fmod(euler.z, 360.0f);

	tempEuler = RdrHelper::Vec3ToRadian(tempEuler);
	RdrHelper::EulerToQuaternion(quaternion, tempEuler);
}

void RdrHelper::EulerToQuaternion(RDRQUAT& quaternion, const RDRVEC3& euler)
{
#ifdef USE_D3D9
	D3DXQuaternionRotationYawPitchRoll(&quaternion, euler.y, euler.x, euler.z);
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
#elif defined(USE_OPENGL)
	quaternion = glm::quat(euler);
#endif
}

void RdrHelper::QuaternionToEuler(RDRVEC3& euler, RDRQUAT& q)
{
#ifdef USE_D3D9
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
#endif
}

void RdrHelper::AngleTo(RDRVEC3& angle, const RDRVEC3& from, const RDRVEC3& location)
{
#ifdef USE_D3D9
    D3DXVECTOR3 v3;
	D3DXVec3Subtract(&v3, &location, &from);
	D3DXVec3Normalize(&v3, &v3);
    angle.x = asin(v3.y);
    angle.y = atan2(-v3.x, -v3.z);
#endif
}

void RdrHelper::Vec4ToVec3(RDRVEC3& result, RDRVEC4& v4)
{
	result.x = v4.x / v4.w; result.y = v4.y / v4.w; result.z = v4.z / v4.w;
}

RDRVEC3 RdrHelper::Vec3ToRadian(const RDRVEC3& input)
{
	RDRVEC3 output;
	output.x = (float)ToRadian(input.x);
	output.y = (float)ToRadian(input.y);
	output.z = (float)ToRadian(input.z);
	return output;
}

RDRVEC3 RdrHelper::Vec3ToDegree(const RDRVEC3& input)
{
	RDRVEC3 output;
	output.x = (float)ToDegree(input.x);
	output.y = (float)ToDegree(input.y);
	output.z = (float)ToDegree(input.z);
	return output;
}

RDRQUAT RdrHelper::RotationBetweenVectors(RDRVEC3 start, RDRVEC3 dest) {
#ifdef USE_OPENGL
    if (!(start.x == 0 && start.y == 0 && start.z == 0))
        start = glm::normalize(start);
    if (!(dest.x == 0 && dest.y == 0 && dest.z == 0))
        dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length(rotationAxis) < 0.01 )
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);
		rotationAxis = glm::normalize(rotationAxis);
		return glm::angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = glm::cross(start, dest);

	float s = sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
#endif
}

RDRQUAT RdrHelper::LookAt(const RDRVEC3& position, const RDRVEC3& target, const RDRVEC3& up)
{
#ifdef USE_OPENGL
    RDRVEC3 forwardVec = glm::normalize(target - position);
    float dot = glm::dot(VECTOR_FORWARD, forwardVec);
    if (glm::abs(dot - (-1.0f) < 0.000001f)) {
        return glm::quat(M_PI, up.x, up.y, up.z);
    }
    if (glm::abs(dot - 1.0f) < 0.000001f) {
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }

    float angle = acosf(dot);
    RDRVEC3 axis = glm::normalize(glm::cross(VECTOR_FORWARD, forwardVec));
    return glm::angleAxis(angle, axis);
#endif
}
