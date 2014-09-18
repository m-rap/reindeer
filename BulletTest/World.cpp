#include "World.h"

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

	if (!glewIsSupported("GL_VERSION_1_5")) {
		printf("glGenBuffers is not supported\n");
		return;
	}

	printf("%s\n", glGetString(GL_VERSION));
}

World::~World()
{
    //dtor
}

void World::Init()
{
    lightPosition[0] = 50.0f;
    lightPosition[1] = 50.0f;
    lightPosition[2] = -50.0f;
    programId = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	lightPositionId = glGetUniformLocation(programId, "LightPosition_worldspace");

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
	dynamicsWorld->setGravity(btVector3(0,-10,0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
	btRigidBody::btRigidBodyConstructionInfo
			groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	for (size_t i = 0; i < DrawableObjects.size(); i++)
        ((BoxObject*)DrawableObjects[i])->SetDynamicsWorld(dynamicsWorld);

    glUseProgram(programId);
    glUniform3f(lightPositionId, lightPosition[0], lightPosition[1], lightPosition[2]);
    while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dynamicsWorld->stepSimulation(1/60.f,10);

		for (size_t i = 0; i < DrawableObjects.size(); i++)
        {
            DrawableObjects[i]->Draw(camera);
        }

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;

	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;

	glDeleteProgram(programId);

	glfwTerminate();
}
