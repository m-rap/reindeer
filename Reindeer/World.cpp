#include "World.h"

#ifndef USE_OPENGL
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
#else
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	World::Global->ScrollCallback(window, xoffset, yoffset);
}
#endif

World* World::Global = NULL;

World::World()
{
	mouseRightButtonDown = false;
	mouseMiddleButtonDown = false;
	btClock cl;
	lastTimeMiddleMousePressed = cl.getTimeMilliseconds();
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
#ifndef USE_OPENGL
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

	glfwSetScrollCallback(window, scroll_callback);
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

	ground = new BoxObject();
	ground->SetMinMax(RDRVEC3(-25.0f, -25.0f, -25.0f), RDRVEC3(25.0f, 25.0f, 25.0f));
	ground->SetPosition(RDRVEC3(0.0f, -25.0f, 0.0f));
	ground->SetMass(0.0f);
	dynamicsWorld->addRigidBody(ground->rigidBody);

	for (size_t i = 0; i < PhysicalObjects.size(); i++)
	{
		PhysicalObjects[i]->SetDynamicsWorld(dynamicsWorld);
		dynamicsWorld->addRigidBody(PhysicalObjects[i]->rigidBody);
	}

	btClock cl;
	btScalar currentTime = (btScalar)cl.getTimeMicroseconds() / 1000000.f;

#ifndef USE_OPENGL
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
		
		int mouseRightButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (mouseRightButtonState == GLFW_PRESS && !mouseRightButtonDown) {
			mouseRightButtonDown = true;
			glfwGetCursorPos(window, &mouseX, &mouseY);
		}
		else if (mouseRightButtonState == GLFW_RELEASE) {
			mouseRightButtonDown = false;
		}

		int mouseMiddleButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
		if (mouseMiddleButtonState == GLFW_PRESS) {
			lastTimeMiddleMousePressed = cl.getTimeMilliseconds();
			if (!mouseMiddleButtonDown) {
				mouseMiddleButtonDown = true;
				glfwGetCursorPos(window, &mouseX, &mouseY);
			}
		}

		unsigned long tempMid = cl.getTimeMilliseconds() - lastTimeMiddleMousePressed;
		if (tempMid > 50 && mouseRightButtonState == GLFW_RELEASE) {
			mouseMiddleButtonDown = false;
		}

		if (mouseRightButtonDown) {
			double newMouseX, newMouseY;
			glfwGetCursorPos(window, &newMouseX, &newMouseY);
			camera.RotateLocalX(ToRadian((mouseY - newMouseY) * 0.1f), true);
			camera.RotateGlobalY(ToRadian((mouseX - newMouseX) * 0.1f), true);
			camera.BuildWorld();
			mouseX = newMouseX;
			mouseY = newMouseY;
		}

		if (mouseMiddleButtonDown) {
			double newMouseX, newMouseY;
			glfwGetCursorPos(window, &newMouseX, &newMouseY);
			RDRVEC3 pos = *camera.GetPosition();
			RDRVEC3 delta = (float)(mouseX - newMouseX) * 0.01f * glm::normalize(*camera.GetRotation() * AXIS_X);
			delta += (float)(newMouseY - mouseY) * 0.01f * glm::normalize(*camera.GetRotation() * AXIS_Y);
			camera.SetPosition(pos + delta);
			mouseX = newMouseX;
			mouseY = newMouseY;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			camera.RotateGlobalY(ToRadian(0.01f));

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			camera.RotateGlobalY(ToRadian(-0.01f));
		
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera.RotateLocalX(ToRadian(0.01f));

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera.RotateLocalX(ToRadian(-0.01f));

		btScalar newTime = (btScalar)cl.getTimeMicroseconds() / 1000000.f;
		btScalar frameTime = newTime - currentTime;
		currentTime = newTime;

		dynamicsWorld->stepSimulation(frameTime, 7);

		for (size_t i = 0; i < PhysicalObjects.size(); i++)
		{
			//PhysicalObject& obj = *PhysicalObjects[i];
			//RDRQUAT& rot = *obj.GetRotation();
			//RDRVEC3 delta = rot * glm::vec3(0.0f, 0.0f, 0.001f);
			//obj.SetPosition(*obj.GetPosition() + delta);
			PhysicalObjects[i]->Update();
		}

		PreUpdate();

		Draw();

		PostUpdate();

#ifndef USE_OPENGL
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
	dynamicsWorld->removeRigidBody(ground->rigidBody);
	//dynamicsWorld->removeRigidBody(groundRigidBody);

	DrawableObjects.clear();
	PhysicalObjects.clear();

    delete ground;
	//delete groundRigidBody->getMotionState();
	//delete groundRigidBody;
	//delete groundShape;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	for (unordered_map<string, btCollisionShape*>::iterator it = CollisionShapes.begin(); it != CollisionShapes.end(); it++)
	{
		delete it->second;
	}
	CollisionShapes.clear();

	PostRender();

#ifdef _MSC_VER
#else
	glfwTerminate();
#endif
}

void World::Draw()
{
	for (size_t i = 0; i < DrawableObjects.size(); i++)
    {
		DrawableObjects[i]->Draw(&camera, light);
    }
    ground->Draw(&camera, light);
}

void World::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	RDRVEC3 pos = *camera.GetPosition();
	RDRVEC3 delta = (float)yoffset * glm::normalize(*camera.GetRotation() * AXIS_Z);
	camera.SetPosition(pos + delta);
}