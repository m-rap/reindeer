#pragma once
#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include "BaseObject.h"

#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

class PhysicalObject :
	public BaseObject
{
protected:
    float mass;

	virtual void BuildShape() = 0;
	void BuildRigidBody();
	void DeleteRigidBody();
	void SetRigidBodyTransform();

public:
	PhysicalObject(void);
	virtual ~PhysicalObject(void);

	btRigidBody* rigidBody;
    btCollisionShape* collisionShape;
    btDiscreteDynamicsWorld* dynamicsWorld;

	void SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	void Update();

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

	void SetMass(const float& mass);
};

#endif
