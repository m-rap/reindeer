#include "reindeer.h"
#include "RdrWorld.h"
#include "RdrCollisionShapeFactory.h"

RdrWorld* RdrWorld::Global = 0;

RdrWorld::RdrWorld(Container* container)
{
    mouseRightButtonDown = false;
    mouseMiddleButtonDown = false;
    btClock cl;
    lastTimeMiddleMousePressed = cl.getTimeMilliseconds();
    this->container = container;
    container->AddListener(this);
}

RdrWorld::~RdrWorld()
{
    //dtor
}

void RdrWorld::Init(int argc, char *argv[])
{
    InitWindow(argc, argv);
    Init3d();
}

void RdrWorld::InitWindow(int argc, char *argv[])
{
    container->Init(argc, argv);
}

void TraverseRigidBodies(btCollisionShape** result, RdrNode* root, RdrNode* current, int& bodiesCount, float& totalMass)
{
    for (deque<RdrRigidBodyInfo*>::iterator it = current->RigidBodyInfos.begin(); it != current->RigidBodyInfos.end(); it++) {
        bodiesCount++;
        totalMass += (*it)->mass;

        if (bodiesCount < 2) {
            *result = (*it)->shape;
            continue;
        }
        if (bodiesCount == 2) {
            *result = RdrCollisionShapeFactory::GetInstance().CreateCompoundShape(root);
        }
        btTransform tr;
        RdrTransform* rdrTr = current->GetTransform();
        RDRVEC3* pos = rdrTr->GetPosition();
        RDRQUAT* rot = rdrTr->GetRotation();
        tr.setOrigin(btVector3(pos->x * PHYSICS_WORLD_SCALE, pos->y * PHYSICS_WORLD_SCALE, pos->z * PHYSICS_WORLD_SCALE));
        tr.setRotation(btQuaternion(rot->x, rot->y, rot->z, rot->w));
        ((btCompoundShape*)*result)->addChildShape(tr, (*it)->shape);
    }

    for (deque<RdrNode*>::iterator it = current->GetChildren()->begin(); it != current->GetChildren()->end(); it++) {
        TraverseRigidBodies(result, root, *it, bodiesCount, totalMass);
    }
}

void RdrWorld::Render()
{
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    collisionConfiguration->setConvexConvexMultipointIterations(3);
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0.0f * PHYSICS_WORLD_SCALE, -10.0f * PHYSICS_WORLD_SCALE, 0.0f * PHYSICS_WORLD_SCALE));

    deque<btRigidBody*> rigidBodies;

    for (deque<RdrNode*>::iterator it = nodes.begin(); it != nodes.end(); it++) {
        btCollisionShape* shape;
        int bodiesCount = 0;
        float totalMass = 0;
        TraverseRigidBodies(&shape, *it, *it, bodiesCount, totalMass);

        if (shape == NULL)
            continue;

        // build the rigid body
        RdrTransform* rdrTr = (*it)->GetTransform();
        RDRQUAT* rotation = rdrTr->GetRotation();
        RDRVEC3* position = rdrTr->GetPosition();
        btTransform tr;
        tr.setIdentity();
        tr.setRotation(btQuaternion(rotation->x, rotation->y, rotation->z, rotation->w));
        tr.setOrigin(btVector3(position->x * PHYSICS_WORLD_SCALE, position->y * PHYSICS_WORLD_SCALE, position->z * PHYSICS_WORLD_SCALE));

        btDefaultMotionState* motionState = new btDefaultMotionState(tr);

        btVector3 inertia;
        shape->calculateLocalInertia(totalMass, inertia);

        btRigidBody::btRigidBodyConstructionInfo constructionInfo(totalMass, motionState, shape, inertia);
        constructionInfo.m_friction = 1.0f;
        constructionInfo.m_restitution = 0.0f;

        btRigidBody* rb = new btRigidBody(constructionInfo);
        rb->setUserPointer(*it);
        rigidBodies.push_back(rb);
        dynamicsWorld->addRigidBody(rb);
    }

    btClock cl;
    btScalar currentTime = (btScalar)cl.getTimeMicroseconds() / 1000000.f;

    while (true) {
        container->ReadInput();
        if (container->ShouldClose())
            break;

        btScalar newTime = (btScalar)cl.getTimeMicroseconds() / 1000000.f;
        btScalar frameTime = newTime - currentTime;
        currentTime = newTime;

        dynamicsWorld->stepSimulation(frameTime, 7);

        for (deque<btRigidBody*>::iterator it = rigidBodies.begin(); it != rigidBodies.end(); it++)
        {
            RdrNode* rc = (RdrNode*)(*it)->getUserPointer();
            btTransform trans;
            (*it)->getMotionState()->getWorldTransform(trans);
            rc->Update(trans);
        }

        PreUpdate();
        Draw();
        PostUpdate();
        container->PostUpdate();
    }

    for (deque<btRigidBody*>::iterator it = rigidBodies.begin(); it != rigidBodies.end(); it++)
    {
        dynamicsWorld->removeRigidBody(*it);
        delete *it;
    }

    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;

    PostRender();
}

void RdrWorld::Draw()
{
	for (deque<RdrNode*>::iterator it = nodes.begin(); it != nodes.end(); it++) {
		(*it)->Draw(NULL, &camera, light);
    }
}

void RdrWorld::Scrolled(double xoffset, double yoffset)
{
    RdrTransform* tr = camera.GetTransform();
    RDRVEC3 pos = *tr->GetPosition();
	RDRVEC3 delta = (float)yoffset * RdrHelper::Vec3Normalize(RdrHelper::Vec3Transform(*tr->GetRotation(), AXIS_Z));
	tr->SetPosition(pos + delta);
	camera.BuildView();
}

void RdrWorld::MouseLeftButtonPressed(double x, double y)
{
}

void RdrWorld::MouseLeftButtonReleased(double x, double y)
{
}

void RdrWorld::MouseRightButtonPressed(double x, double y)
{
    mouseRightButtonDown = true;
    mouseX = x;
    mouseY = y;
}

void RdrWorld::MouseRightButtonReleased(double x, double y)
{
    mouseRightButtonDown = false;
}

void RdrWorld::MouseMiddleButtonPressed (double x, double y)
{
    mouseMiddleButtonDown = true;
    mouseX = x;
    mouseY = y;
}

void RdrWorld::MouseMiddleButtonReleased(double x, double y)
{
    mouseMiddleButtonDown = false;
}

void RdrWorld::MouseMoved(double x, double y)
{
    RdrTransform* tr = camera.GetTransform();
    if (mouseRightButtonDown) {
		double deltaX = mouseX - x, deltaY = mouseY - y;
		printf("zz %lf %lf\n", deltaX, deltaY);
        tr->RotateLocalX(ToRadian(deltaY * 0.1f), true);
        tr->RotateGlobalY(ToRadian(deltaX * 0.1f), true);
        tr->BuildWorld();
        camera.BuildView();
        mouseX = x;
        mouseY = y;
    }

    if (mouseMiddleButtonDown) {
		double deltaX = mouseX - x, deltaY = mouseY - y;
		printf("zz %lf %lf\n", deltaX, deltaY);
        RDRVEC3 pos = *tr->GetPosition();
        RDRVEC3 delta = (float)deltaX * 0.01f * RdrHelper::Vec3Normalize(RdrHelper::Vec3Transform(*tr->GetRotation(), AXIS_X));
		delta += (float)-deltaY * 0.01f * RdrHelper::Vec3Normalize(RdrHelper::Vec3Transform(*tr->GetRotation(), AXIS_Y));
        tr->SetPosition(pos + delta);
        camera.BuildView();
        mouseX = x;
        mouseY = y;
    }
}

void RdrWorld::KeyPressed(int keyCode)
{
    RdrTransform* tr = camera.GetTransform();
    if (keyCode == container->KEY_LEFT())
        tr->RotateGlobalY(ToRadian(0.01f));

    if (keyCode == container->KEY_RIGHT())
        tr->RotateGlobalY(ToRadian(-0.01f));

    if (keyCode == container->KEY_UP())
        tr->RotateLocalX(ToRadian(0.01f));

    if (keyCode == container->KEY_DOWN())
        tr->RotateLocalX(ToRadian(-0.01f));

    if (keyCode == container->KEY_M())
        DEBUG_SHADOWMAP = !DEBUG_SHADOWMAP;

    camera.BuildView();
}

void RdrWorld::KeyReleased(int keyCode)
{
}
