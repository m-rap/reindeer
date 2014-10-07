#define _USE_MATH_DEFINES
#include <math.h>

#ifdef USE_D3D9
#include <d3d9.h>
#include <d3dx9.h>

struct CUSTOMVERTEX {FLOAT X, Y, Z; D3DVECTOR NORMAL;};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_NORMAL)

extern LPDIRECT3DDEVICE9 d3ddev;
#else

#include <GL/glew.h>
#ifdef _MSC_VER
#include <GL/wglew.h>
#endif
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#endif

#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#ifdef _MSC_VER
#include <windows.h>
#include <windowsx.h>
#else
#include <GLFW/glfw3.h>
#endif

#include <iostream>
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <time.h>
#include <vector>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern float PHYSICS_WORLD_SCALE;

#include "RdrHelper.h"