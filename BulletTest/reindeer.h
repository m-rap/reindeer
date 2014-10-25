#ifndef REINDEER_H
#define REINDEER_H

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

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#ifndef _MSC_VER
#include <unistd.h>
#else
#define snprintf _snprintf
#endif

using namespace std;

#include "BulletCollision/CollisionShapes/btCollisionShape.h"

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern float PHYSICS_WORLD_SCALE;
extern unordered_map<string, btCollisionShape*> CollisionShapes;

#include "RdrHelper.h"

// untuk test legacy opengl di device dengan versi 1.5 ke atas uncomment di bawah ini
//#ifdef GLEW_VERSION_1_5
//#undef GLEW_VERSION_1_5
//#endif
//#define GLEW_VERSION_1_5 0

#endif
