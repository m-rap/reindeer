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

void RdrHelper::Mat4Multiply(RDRMAT4& output, const RDRMAT4& a, const RDRMAT4& b)
{
#ifdef USE_D3D9
	D3DXMatrixMultiply(&output, &b, &a);
#else
	output = a * b;
#endif
}

void RdrHelper::Mat4Multiply(RDRMAT4& output, const RDRMAT4& a, const RDRMAT4& b, const RDRMAT4& c)
{
#ifdef USE_D3D9
	D3DXMatrixIdentity(&output);
	D3DXMatrixMultiply(&output, &output, &c);
	D3DXMatrixMultiply(&output, &output, &b);
	D3DXMatrixMultiply(&output, &output, &a);
#else
	output = a * b * c;
#endif
}

RDRQUAT RdrHelper::QuatMultiply(const RDRQUAT& a, const RDRQUAT& b)
{
#ifdef USE_D3D9
	RDRQUAT quat;
	D3DXQuaternionMultiply(&quat, &b, &a);
	return quat;
#else
	return a * b;
#endif
}

void RdrHelper::BuildWorld(RDRMAT4& world, const RDRVEC3& position, const RDRQUAT& rotation, const RDRVEC3& scale)
{
#ifdef USE_D3D9
	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity(&world);

	// scale
	D3DXMatrixScaling(&tempMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&world, &world, &tempMatrix);

	// rotation
	D3DXMatrixRotationQuaternion(&tempMatrix, &rotation);
	D3DXMatrixMultiply(&world, &world, &tempMatrix);

	// position
	D3DXMatrixTranslation(&tempMatrix, position.x, position.y, position.z);
	D3DXMatrixMultiply(&world, &world, &tempMatrix);

#elif defined( USE_OPENGL )
	world = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);
#endif
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
	forward3 = Vec4ToVec3(forward);

    D3DXVECTOR4 up;
	D3DXVECTOR3 up3;
	D3DXVec3Transform(&up, &VECTOR_UP, &tempMat);
	up3 = Vec4ToVec3(up);

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
		up3 = Vec4ToVec3(up);

		D3DXMatrixRotationX(&tempMat, -euler.x);
        D3DXVec3Transform(&up, &up3, &tempMat);
		up3 = Vec4ToVec3(up);

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

RDRVEC3 RdrHelper::Vec4ToVec3(RDRVEC4& v4)
{
	RDRVEC3 result;
	result.x = v4.x / v4.w; result.y = v4.y / v4.w; result.z = v4.z / v4.w;
	return result;
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

RDRVEC3 RdrHelper::Vec3Normalize(const RDRVEC3& input)
{
#ifdef USE_D3D9
	RDRVEC3 res;
	D3DXVec3Normalize(&res, &input);
	return res;
#else
	return glm::normalize(input);
#endif
}

RDRVEC3 RdrHelper::Vec3Transform(const RDRQUAT& rotation, const RDRVEC3& vec)
{
#ifdef USE_D3D9
	D3DXMATRIX tempMat;
	D3DXMatrixRotationQuaternion(&tempMat, &rotation);
	D3DXVECTOR4 res;
	D3DXVec3Transform(&res, &vec, &tempMat);
	return Vec4ToVec3(res);
#else
	return rotation * vec;
#endif
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
#else
	return RDRQUAT(0.0f, 0.0f, 0.0f, 1.0f);
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
#else
	RDRVEC3 forwardVec = Vec3Normalize(target - position);
	float dot = D3DXVec3Dot(&VECTOR_FORWARD, &forwardVec);
	if (abs(dot - (-1.0f) < 0.000001f)) {
		return RDRQUAT(up.x, up.y, up.z, M_PI);
	}
	if (abs(dot - 1.0f) < 0.000001f) {
		return RDRQUAT(0.0f, 0.0f, 0.0f, 1.0f);
	}

	float angle = acosf(dot);
	RDRVEC3 axis;
	D3DXVec3Cross(&axis, &VECTOR_FORWARD, &forwardVec);
	axis = Vec3Normalize(axis);
#endif

	return AngleAxis(angle, axis);
}

RDRQUAT RdrHelper::AngleAxis(const float& angle, const RDRVEC3& axis)
{
#ifdef USE_OPENGL
	return glm::angleAxis(angle, axis);
#else
	RDRQUAT quat;
	D3DXQuaternionRotationAxis(&quat, &axis, angle);
	return quat;
#endif
}