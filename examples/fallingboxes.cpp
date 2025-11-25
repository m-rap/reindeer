#ifdef USE_D3D9
#include "D3d9World.h"
#else
#include "RdrWorld_OpenGL.h"
#include "RdrMeshFactory_OpenGL.h"
#endif
#include "RdrCollisionShapeFactory.h"
#ifdef USE_GLFW
#include "../container/GlfwContainer.h"
#else
#include "../container/GlxContainer.h"
#endif
#ifdef USE_GTK
#include "../container/RdrGtkContainer.h"
#endif
#ifdef _WIN32
#include "WglContainer.h"
#endif

using namespace std;

int main(int argc, char *argv[])
{
    int nBox;
    //printf("nBox: ");
    nBox = 10;
    //scanf("%d", &nBox);

    //printf("scale: ");
    PHYSICS_WORLD_SCALE = 1;
    //scanf("%f", &PHYSICS_WORLD_SCALE);

#ifdef USE_D3D9
    Win32Container container;
    D3d9World world(&container);
#else
    //GlfwContainer container;
    //WglContainer container;
    GlxContainer container;
    //RdrGtkContainer container;
    RdrWorld_OpenGL world(&container);
#endif
    RdrWorld::Global = &world;
    container.Init(argc, argv);

    RdrNode* nodes = new RdrNode[nBox + 2];

    btCollisionShape* groundShape = RdrCollisionShapeFactory::GetInstance().CreateBoxShape(RDRVEC3(-25.0f, -25.0f, -25.0f), RDRVEC3(25.0f, 25.0f, 25.0f));

    nodes[0].AddRigidBodyInfo(groundShape, 0.0f);
    nodes[0].GetTransform()->SetPosition(RDRVEC3(0.0f, -25.0f, 0.0f));
    world.nodes.push_back(&nodes[0]);

    RDRVEC3 boxmin(-0.1f, -0.1f, -0.1f);
    RDRVEC3 boxmax( 0.1f,  0.1f,  0.1f);

    RdrMesh* box = RdrMeshFactory_OpenGL::GetInstance().CreateBoxMesh(boxmin, boxmax);
    btCollisionShape* boxShape = RdrCollisionShapeFactory::GetInstance().CreateBoxShape(boxmin, boxmax);

    for (int i = 1; i < nBox + 1; i++)
    {
        nodes[i].Mesh = box;
        nodes[i].AddRigidBodyInfo(boxShape, 1);
        nodes[i].GetTransform()->SetPosition(RDRVEC3(-1.0f + i*0.1f, 10.0f + i*1.0f, 0.0f), true);
        nodes[i].GetTransform()->RotateGlobalX(ToRadian(44.0f), true);
        nodes[i].GetTransform()->RotateGlobalY(ToRadian(10.0f), true);
        nodes[i].GetTransform()->RotateGlobalZ(ToRadian(30.0f), true);
        nodes[i].GetTransform()->BuildWorld();
        world.nodes.push_back(&nodes[i]);
    }


    //RdrMesh* suzanne = RdrMeshFactory_OpenGL::GetInstance().CreateComplexMesh("res/suzanne.obj");
    //btCollisionShape* suzanneShape = RdrCollisionShapeFactory::GetInstance().CreateComplexShape("res/suzanne.obj");
    //
    //nodes[nBox + 1].Mesh = suzanne;
    //nodes[nBox + 1].AddRigidBodyInfo(suzanneShape, 7.0f);
    //nodes[nBox + 1].GetTransform()->SetPosition(RDRVEC3(-0.7f, 1.0f, 0.5f), true);
    ////nodes[nBox].GetTransform()->RotateGlobalY(ToRadian(180.0f), true);
    //nodes[nBox + 1].GetTransform()->BuildWorld();
    //world.nodes.push_back(&nodes[nBox + 1]);

    RdrCamera& camera = world.camera;
    camera.GetTransform()->SetPosition(RDRVEC3(0.0f, 3.0f, 7.0f), true);
    //camera.GetTransform()->RotateLocalY(ToRadian(180.0f), true);
    //camera.GetTransform()->RotateLocalX(ToRadian(10), true);
    camera.GetTransform()->BuildWorld();
    camera.BuildView();

    world.CreatePhysicsEnv();

    container.Main();
    container.Deinit();

    delete[] nodes;

    return 0;
}
