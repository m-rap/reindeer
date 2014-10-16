#pragma once

#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "BaseObject.h"
#include "OpenGLModelRenderer.h"

#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

class ModelObject :
	public BaseObject
{
private:
	ModelRenderer* renderer;
	void BuildRigidBody();
	void DeleteRigidBody();
	void SetRigidBodyTransform();
public:
	ModelObject(const char* modelPath);
	virtual ~ModelObject(void);

	std::vector<RDRVEC3> vertices;
	std::vector<RDRVEC2> uvs;
	std::vector<RDRVEC3> normals;

	btRigidBody* rigidBody;
    btCollisionShape* collisionShape;
    btDiscreteDynamicsWorld* dynamicsWorld;

#ifdef USE_OPENGL
	void SetProgramId(GLuint programId);
#endif
	void LoadModel(const char* modelPath);
	void SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	void Update();
	void Draw(Camera* camera);

    virtual void SetPosition(const RDRVEC3& position, bool silent = false)
    {
    	BaseObject::SetPosition(position, silent);
    	SetRigidBodyTransform();
    }

	virtual void SetEuler(const RDRVEC3& euler, bool silent = false)
	{
		BaseObject::SetEuler(euler, silent);
		SetRigidBodyTransform();
	}
};

#endif
