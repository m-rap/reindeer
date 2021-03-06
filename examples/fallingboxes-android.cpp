#include "RdrWorld_OpenGL.h"
#include "RdrMeshFactory_OpenGL.h"
#include "RdrCollisionShapeFactory.h"
#include "../container/AndroidEglContainer.h"

using namespace std;

void constructDraw(Container* _container, RdrWorld* world) {
    LOGI("inside constructDraw");
    int nBox;
    //printf("nBox: ");
    nBox = 10;
    //scanf("%d", &nBox);

    //printf("scale: ");
    PHYSICS_WORLD_SCALE = 1;
    //scanf("%f", &PHYSICS_WORLD_SCALE);

    //RdrWorld_OpenGL world(&container);
    RdrWorld::Global = world;

    //vector<RdrNode> nodes;
    //nodes.resize(nBox + 2);

    static RdrNode nodes[11];

    LOGI("create ground %08X", &nodes[0]);

    btCollisionShape* groundShape = RdrCollisionShapeFactory::GetInstance().CreateBoxShape(RDRVEC3(-25.0f, -25.0f, -25.0f), RDRVEC3(25.0f, 25.0f, 25.0f));

    nodes[0].AddRigidBodyInfo(groundShape, 0.0f);
    nodes[0].GetTransform()->SetPosition(RDRVEC3(0.0f, -25.0f, 0.0f));
    world->nodes.push_back(&nodes[0]);

    RDRVEC3 boxmin(-0.1f, -0.1f, -0.1f);
    RDRVEC3 boxmax( 0.1f,  0.1f,  0.1f);

    LOGI("create boxes");

    RdrMesh* box = RdrMeshFactory_OpenGL::GetInstance().CreateBoxMesh(boxmin, boxmax);
    btCollisionShape* boxShape = RdrCollisionShapeFactory::GetInstance().CreateBoxShape(boxmin, boxmax);

    for (int i = 1; i < nBox + 1; i++)
    {
        LOGI("Setup box %d %08X", i, &nodes[i]);
        nodes[i].Mesh = box;
        nodes[i].AddRigidBodyInfo(boxShape, 1);
        nodes[i].GetTransform()->SetPosition(RDRVEC3(-1.0f + i*0.1f, 10.0f + i*1.0f, 0.0f), true);
        nodes[i].GetTransform()->RotateGlobalX(ToRadian(44.0f), true);
        nodes[i].GetTransform()->RotateGlobalY(ToRadian(10.0f), true);
        nodes[i].GetTransform()->RotateGlobalZ(ToRadian(30.0f), true);
        nodes[i].GetTransform()->BuildWorld();
        world->nodes.push_back(&nodes[i]);
    }


//    RdrMesh* suzanne = RdrMeshFactory_OpenGL::GetInstance().CreateComplexMesh("res/suzanne.obj");
//    btCollisionShape* suzanneShape = RdrCollisionShapeFactory::GetInstance().CreateComplexShape("res/suzanne.obj");
//
//    nodes[nBox + 1].Mesh = suzanne;
//    nodes[nBox + 1].AddRigidBodyInfo(suzanneShape, 7.0f);
//    nodes[nBox + 1].GetTransform()->SetPosition(RDRVEC3(-0.7f, 1.0f, 0.5f), true);
//    //nodes[nBox].GetTransform()->RotateGlobalY(ToRadian(180.0f), true);
//    nodes[nBox + 1].GetTransform()->BuildWorld();
//    world.nodes.push_back(&nodes[nBox + 1]);

    LOGI("setup camera");

    RdrCamera& camera = world->camera;
    camera.GetTransform()->SetPosition(RDRVEC3(0.0f, 3.0f, 7.0f), true);
    //camera.GetTransform()->RotateLocalY(ToRadian(180.0f), true);
    //camera.GetTransform()->RotateLocalX(ToRadian(10), true);
    camera.GetTransform()->BuildWorld();
    camera.BuildView();

    LOGI("call CreatePhysicsEnv");

    world->CreatePhysicsEnv();
}

void android_main(struct android_app* state) {
    LOGI("USE_LEGACY %d", USE_LEGACY);
    AndroidEglContainer container;
    //RdrWorld_OpenGL world(&container);
    RdrWorld_OpenGL world(&container);

    LOGI("call SetAttrs");
    container.SetAttrs(state, &constructDraw, &world);

    LOGI("call Main");
    container.Main();
}
