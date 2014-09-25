#include "World.h"

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

World::World()
{
#ifdef _MSC_VER	
	HWND hWnd;
    WNDCLASSEX wc;
	HGLRC hRC;
	PIXELFORMATDESCRIPTOR pfd;
	int pixelformat;

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
	
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	//pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    //pfd.nVersion = 1;
    //pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    
	pixelformat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, pixelformat, &pfd);
	
	hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC,hRC);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
#else
    int init = glfwInit();
	if (init == GL_FALSE) {
		printf("glfw init failed\n");
		return;
	}

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Our OpenGL Program", NULL, NULL);
	if (!window) {
		printf("glfw create window failed\n");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
#endif

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
		return;
	}

	/*if (!GLEW_VERSION_1_5) {
		printf("glGenBuffers is not supported\n");
		return;
	}*/

	printf("%s\n", glGetString(GL_VERSION));
	printf("1.5 %d\n", GL_VERSION_1_5);
}

World::~World()
{
    //dtor
}

void World::Init()
{
    lightPosition[0] = -3.0f;
    lightPosition[1] = 3.0f;
    lightPosition[2] = -3.0f;
    lightPosition[3] = 0.0f;

    if (GLEW_VERSION_1_5)
	{
		programId = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
		lightPositionId = glGetUniformLocation(programId, "LightPosition_worldspace");
	}
	else
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		//float lightAmbient[] = { 50.0f/255.0f, 50.0f/255.0f, 50.0f/255.0f, 1.0f };
		float lightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
		//float lightDirection[] = { -2.0f, -2.0f, -3.0f };
		//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		lightPosition[2] *= -1;
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.125f);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

		float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);

		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
	glEnable(GL_CULL_FACE);
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

	for (size_t i = 0; i < DrawableObjects.size(); i++)
	{
		((BoxObject*)DrawableObjects[i])->SetDynamicsWorld(dynamicsWorld);
		dynamicsWorld->addRigidBody(((BoxObject*)DrawableObjects[i])->rigidBody);
	}

    if (GLEW_VERSION_1_5)
	{
		glUseProgram(programId);
		glUniform3f(lightPositionId, lightPosition[0], lightPosition[1], lightPosition[2]);
	}

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
		clock_t c1 = clock();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dynamicsWorld->stepSimulation(1/60.f);//, 10, 1.0f/240.0f);

		for (size_t i = 0; i < DrawableObjects.size(); i++)
        {
            DrawableObjects[i]->Draw(&camera);
        }
		
#ifdef _MSC_VER
		SwapBuffers(hDC);
		long span = (clock() - c1) / CLOCKS_PER_SEC;
		Sleep(((1000 / 60) - span * 1000));
#else
		glfwSwapBuffers(window);
		glfwPollEvents();
		float span = (clock() - c1) / CLOCKS_PER_SEC;
		usleep(((1000 / 60) - span * 1000) * 1000);
#endif
	}

	for (size_t i = 0; i < DrawableObjects.size(); i++)
	{
		dynamicsWorld->removeRigidBody(((BoxObject*)DrawableObjects[i])->rigidBody);
	}
	dynamicsWorld->removeRigidBody(groundRigidBody);

	delete groundRigidBody->getMotionState();
	delete groundRigidBody;
	delete groundShape;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	if (GLEW_VERSION_1_5)
	{
		glDeleteProgram(programId);
	}

#ifdef _MSC_VER
#else
	glfwTerminate();
#endif
}
