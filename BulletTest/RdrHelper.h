#pragma once

#define ToRadian( degree ) ((degree) * (M_PI / 180.0f))
#define ToDegree( radian ) ((radian) * (180.0f / M_PI))

#ifndef RDRHELPER_H
#define RDRHELPER_H
#ifdef USE_D3D9
#define RDRVEC2 D3DXVECTOR2
#define RDRVEC3 D3DXVECTOR3
#define RDRVEC4 D3DXVECTOR4
#define RDRQUAT D3DXQUATERNION
#define RDRMAT4 D3DXMATRIX
extern const float PIOVER2;
#else
#define RDRVEC2 glm::vec2
#define RDRVEC3 glm::vec3
#define RDRVEC4 glm::vec4
#define RDRQUAT glm::quat
#define RDRMAT4 glm::mat4
#endif

extern const RDRVEC3 VECTOR_UP      ;
extern const RDRVEC3 VECTOR_RIGHT   ;
extern const RDRVEC3 VECTOR_LEFT    ;
extern const RDRVEC3 VECTOR_DOWN    ;
extern const RDRVEC3 VECTOR_BACKWARD;
extern const RDRVEC3 VECTOR_FORWARD ;

class RdrHelper
{
public:
	RdrHelper();
	~RdrHelper();

	static void EulerToQuaternion(RDRQUAT& quaternion, RDRVEC3& euler);
	static void QuaternionToEuler(RDRVEC3& euler, RDRQUAT& quaternion);
	static void AngleTo(RDRVEC3& result, const RDRVEC3& from, const RDRVEC3& location);
	static void Vec4ToVec3(RDRVEC3& result, RDRVEC4& v4);
	static void Vec3ToRadian(RDRVEC3& output, const RDRVEC3& input);
	static void Vec3ToDegree(RDRVEC3& output, const RDRVEC3& input);
};

#endif