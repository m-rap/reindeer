#include "PhysicalObject.h"


PhysicalObject::PhysicalObject(void)
{
	rigidBody = NULL;
	collisionShape = NULL;
	dynamicsWorld = NULL;
	mass = 1.0f;
}

PhysicalObject::~PhysicalObject(void)
{
	DeleteRigidBody();
}

void PhysicalObject::BuildRigidBody()
{
	btTransform tr;
    tr.setIdentity();
    tr.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
    tr.setOrigin(btVector3(position.x * PHYSICS_WORLD_SCALE, position.y * PHYSICS_WORLD_SCALE, position.z * PHYSICS_WORLD_SCALE));

    btDefaultMotionState* motionState = new btDefaultMotionState(tr);

    DeleteRigidBody();

	BuildShape();

    btVector3 inertia;
    collisionShape->calculateLocalInertia(mass, inertia);

    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, collisionShape, inertia);
    constructionInfo.m_friction = 1.0f;
    constructionInfo.m_restitution = 0.0f;
    rigidBody = new btRigidBody(constructionInfo);
}

void PhysicalObject::SetRigidBodyTransform()
{
	btTransform tr;
	tr.setIdentity();
    tr.setRotation(btQuaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w));
    tr.setOrigin(btVector3(position.x * PHYSICS_WORLD_SCALE, position.y * PHYSICS_WORLD_SCALE, position.z * PHYSICS_WORLD_SCALE));
    rigidBody->setCenterOfMassTransform(tr);
}

void PhysicalObject::DeleteRigidBody()
{
	if (rigidBody != NULL)
    {
        delete rigidBody->getMotionState();
        delete rigidBody;
    	collisionShape = NULL;
    }
}

void PhysicalObject::SetDynamicsWorld(btDiscreteDynamicsWorld* dynamicsWorld)
{
    this->dynamicsWorld = dynamicsWorld;
}

void PhysicalObject::Update()
{
    btTransform trans;
    rigidBody->getMotionState()->getWorldTransform(trans);

    btQuaternion rot = trans.getRotation();
#ifdef USE_D3D9
    this->quaternion = RDRQUAT(rot.getX(), rot.getY(), rot.getZ(), rot.getW());
#else
	this->quaternion = RDRQUAT(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
#endif

    btVector3 origin = trans.getOrigin();
    this->position.x = origin.getX() / PHYSICS_WORLD_SCALE;
    this->position.y = origin.getY() / PHYSICS_WORLD_SCALE;
    this->position.z = origin.getZ() / PHYSICS_WORLD_SCALE;

    BuildWorld();
}

void PhysicalObject::SetMass(const float& mass)
{
    this->mass = mass;
    if (rigidBody != NULL) {
        btVector3 inertia;
        rigidBody->getCollisionShape()->calculateLocalInertia(mass, inertia);
        rigidBody->setMassProps(mass, inertia);
    } else {
        BuildRigidBody();
    }
}
