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
	void SetRigidBodyTransformation();

	virtual void TransformationChanged() { SetRigidBodyTransformation(); }

public:
	PhysicalObject(void);
	virtual ~PhysicalObject(void);

	btRigidBody* rigidBody;
    btCollisionShape* collisionShape;
    btDiscreteDynamicsWorld* dynamicsWorld;

	void SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld);
	void Update();

	void SetMass(const float& mass);
};

#endif
