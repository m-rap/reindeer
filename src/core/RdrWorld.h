#ifndef RDRWORLD_H
#define RDRWORLD_H

#include "../container/Container.h"
#include "shader.h"
#include "RdrLight.h"
#include "btBulletDynamicsCommon.h"

using namespace std;

class RdrWorld :
    public ContainerEventListener
{
protected:
    btClock cl;
    btScalar currentTime;

    btBroadphaseInterface* broadphase;
    btDiscreteDynamicsWorld* dynamicsWorld;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;

    deque<btRigidBody*> rigidBodies;

    //BoxObject* ground;

    bool mouseRightButtonDown, mouseMiddleButtonDown;
    unsigned long lastTimeMiddleMousePressed;
    double mouseX, mouseY;

    Container* container;

    virtual void Init3d(int argc, char **argv) = 0;
    virtual void PreRender() {}
    virtual void PreUpdate() {}
    virtual void PostUpdate() {}
    virtual void PostRender() = 0;
    virtual void Draw();

public:
    RdrWorld(Container* container);
    virtual ~RdrWorld();

    static RdrWorld* Global;

    RdrCamera camera;
    RdrLight* light;

    deque<RdrNode*> nodes;

    void Init(int argc, char **argv);
    void CreatePhysicsEnv();

    virtual void OnLoad(int argc, char** argv);
    virtual void OnPaint();
    virtual void OnClosing();
    virtual void OnResize(int width, int height);
    virtual void Scrolled(double xoffset, double yoffset);
    virtual void MouseLeftButtonPressed  (double x, double y);
    virtual void MouseLeftButtonReleased (double x, double y);
    virtual void MouseRightButtonPressed (double x, double y);
    virtual void MouseRightButtonReleased(double x, double y);
    virtual void MouseMiddleButtonPressed (double x, double y);
    virtual void MouseMiddleButtonReleased(double x, double y);
    virtual void KeyPressed(int keyCode);
    virtual void KeyReleased(int keyCode);
    virtual void MouseMoved(double x, double y);
};

#endif // WORLD_H
