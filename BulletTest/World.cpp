#include "World.h"
#include <unistd.h>
#include <time.h>

World::World()
{
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


	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
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
    lightPosition[0] = 1.0f;
    lightPosition[1] = 3.0f;
    lightPosition[2] = -1.0f;
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
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,-9.8,0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	for (size_t i = 0; i < DrawableObjects.size(); i++)
        ((BoxObject*)DrawableObjects[i])->SetDynamicsWorld(dynamicsWorld);

    if (GLEW_VERSION_1_5)
	{
		glUseProgram(programId);
		glUniform3f(lightPositionId, lightPosition[0], lightPosition[1], lightPosition[2]);
	}

	while (!glfwWindowShouldClose(window)) {
		clock_t c1 = clock();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dynamicsWorld->stepSimulation(1/60.f,10);

		for (size_t i = 0; i < DrawableObjects.size(); i++)
        {
            DrawableObjects[i]->Draw(&camera);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
		float span = (clock() - c1) / CLOCKS_PER_SEC;
		usleep(((1000 / 60) - span * 1000) * 1000);
	}

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	if (GLEW_VERSION_1_5)
	{
		glDeleteProgram(programId);
	}

	glfwTerminate();
}
