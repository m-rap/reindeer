#include <iostream>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include "World.h"
#include "BoxObject.h"

using namespace std;

int main()
{
	/*btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0,-10,0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	btCollisionShape* fallShape = new btSphereShape(1);


	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);


	btDefaultMotionState* fallMotionState =
			new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
	btScalar mass = 1;
	btVector3 fallInertia(0,0,0);
	fallShape->calculateLocalInertia(mass,fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
	//btRigidBody::upcast();

	for (int i=0 ; i<300 ; i++) {
        dynamicsWorld->stepSimulation(1/60.f,10);

        btTransform trans;
        btVector3 vel = fallRigidBody->getLinearVelocity();
        fallRigidBody->getMotionState()->getWorldTransform(trans);

        std::cout << "sphere height: " << trans.getOrigin().getY() << " speed: " << vel.getY() << std::endl;
	}

	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;


	delete fallShape;

	delete groundShape;


	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;*/

    World world;
    world.Init();

    glm::vec3 min(-3.0f, -3.0f, -3.0f);
	glm::vec3 max( 3.0f,  3.0f,  3.0f);
	BoxObject box;
	box.SetProgramId(world.programId);
	box.SetMinMax(min, max);
	box.SetPosition(glm::vec3(0.0f, 30.0f, 0.0f), true);
	box.SetEuler(glm::vec3(44,10,30), true);
	box.BuildWorld();

	Camera& camera = world.camera;
	camera.SetPosition(glm::vec3(0.0f, 15.0f, -50.0f), true);
	camera.SetEuler(glm::vec3(0, 0, 0), true);
	camera.BuildWorld();

	world.DrawableObjects.push_back(&box);

	world.Render();


	return 0;
}
