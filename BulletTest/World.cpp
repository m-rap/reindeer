#include "World.h"

#ifdef _MSC_VER
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}
#endif

World::World()
{
}

World::~World()
{
    //dtor
}

void World::Init()
{
	InitWindow();
	Init3d();
}

void World::InitWindow()
{
#ifdef _MSC_VER
    WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(0);
	wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Reindeer", WS_OVERLAPPEDWINDOW, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, GetModuleHandle(0), NULL);
	hDC = GetDC(hWnd);

#ifdef USE_OPENGL
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC hRC;
	int pixelformat;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	pixelformat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelformat, &pfd);

	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC,hRC);
#endif

	ShowWindow(hWnd, SW_SHOWDEFAULT);
#else
    int init = glfwInit();
	if (init == GL_FALSE) {
		printf("glfw init failed\n");
		return;
	}

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Reindeer", NULL, NULL);
	if (!window) {
		printf("glfw create window failed\n");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
#endif
}

void World::Render()
{
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	collisionConfiguration->setConvexConvexMultipointIterations(3);
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0.0f * PHYSICS_WORLD_SCALE, -10.0f * PHYSICS_WORLD_SCALE, 0.0f * PHYSICS_WORLD_SCALE));

	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), 10.0f * PHYSICS_WORLD_SCALE);
	btCollisionShape* groundShape = new btBoxShape(btVector3(50.0f * PHYSICS_WORLD_SCALE, 50.0f * PHYSICS_WORLD_SCALE, 50.0f * PHYSICS_WORLD_SCALE));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0.0f * PHYSICS_WORLD_SCALE, -50.0f * PHYSICS_WORLD_SCALE, 0.0f * PHYSICS_WORLD_SCALE));
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0.0f, 0.0f, 0.0f));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	for (size_t i = 0; i < PhysicalObjects.size(); i++)
	{
		PhysicalObjects[i]->SetDynamicsWorld(dynamicsWorld);
		dynamicsWorld->addRigidBody(PhysicalObjects[i]->rigidBody);
	}

	btClock cl;
	float accumulator = 0.0f, interval = 1.f/60.f;

#ifdef _MSC_VER
	MSG msg;
	while(TRUE)
    {
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)
            break;
#else
	while (!glfwWindowShouldClose(window)) {
#endif
		btScalar dt = (btScalar)cl.getTimeMicroseconds() / 1000000.f;
		cl.reset();
		accumulator += dt;
		while (accumulator >= interval)
		{
			Integrate(dynamicsWorld, interval);
			accumulator -= interval;
		}

		PreUpdate();

		for (size_t i = 0; i < DrawableObjects.size(); i++)
        {
            DrawableObjects[i]->Draw(&camera);
        }

		PostUpdate();

#ifdef _MSC_VER
		SwapBuffers(hDC);
#else
		glfwSwapBuffers(window);
		glfwPollEvents();
#endif
	}

	for (size_t i = 0; i < PhysicalObjects.size(); i++)
	{
		dynamicsWorld->removeRigidBody(PhysicalObjects[i]->rigidBody);
	}
	dynamicsWorld->removeRigidBody(groundRigidBody);

	DrawableObjects.clear();
	PhysicalObjects.clear();

	delete groundRigidBody->getMotionState();
	delete groundRigidBody;
	delete groundShape;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	PostRender();

#ifdef _MSC_VER
#else
	glfwTerminate();
#endif
}

void World::Integrate(btDiscreteDynamicsWorld* dynamicsWorld, float dt)
{
	dynamicsWorld->stepSimulation(dt);//1/60.f, 10, 1.0f/240.0f);
	for (size_t i = 0; i < PhysicalObjects.size(); i++)
	{
		PhysicalObjects[i]->Update();
	}
}