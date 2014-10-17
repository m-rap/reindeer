#ifndef BASEOBJECT_H_
#define BASEOBJECT_H_

#include "reindeer.h"

#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

class BaseObject {
protected:
	RDRVEC3 position;
	RDRVEC3 euler;
	RDRVEC3 scale;
	RDRQUAT quaternion;

	RDRMAT4 rotationMatrix;
	RDRMAT4 world;

public:
	BaseObject();
	virtual ~BaseObject();

	virtual void SetPosition(const RDRVEC3& position, bool silent = false);
	virtual void SetEuler(const RDRVEC3& euler, bool silent = false);
	virtual void SetScale(const RDRVEC3& scale, bool silent = false);

	virtual RDRVEC3* GetPosition();
	virtual RDRVEC3* GetEuler();
	virtual RDRVEC3* GetScale();
	RDRMAT4* GetWorld();

	virtual void BuildWorld();
};

#endif /* BASEOBJECT_H_ */
