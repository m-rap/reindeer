#include "BoxObject.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <time.h>

void sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

void initOpenGL() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//float lightAmbient[] = { 50.0f/255.0f, 50.0f/255.0f, 50.0f/255.0f, 1.0f };
	float lightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float lightPosition[] = { 0.0f, 5.0f, 0.0f, 0.0f };
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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
}

int main() {
	if (!glfwInit()) {
		printf("glfw init failed\n");
		return 0;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Our OpenGL Program", NULL, NULL);
	if (!window) {
		printf("glfw create window failed\n");
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);


//	if (glewInit() != GLEW_OK) {
//		fprintf(stderr, "Failed to initialize GLEW\n");
//		return -1;
//	}


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	initOpenGL();


	// [rdr_note] Rdr objects instantiated here

	glm::vec3 min(-3.0f, -3.0f, -3.0f);
	glm::vec3 max( 3.0f,  3.0f,  3.0f);
	BoxObject box;
	box.SetMinMax(min, max);
	glm::vec3 position(10.0f, 0.0f, 0.0f), rotation(30, 0, 0);
	box.SetPosition(position);

	Camera camera;
	position.x = 0.0f;
	position.y = 30.0f;
	position.z = -30.0f;
	camera.SetPosition(position);
	camera.SetEuler(rotation);


	// enter the main loop:

	float i = 0;
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// [rdr_note] modify Rdr objects behavior

		glm::vec3 tempPosition((float)sin(i) * 12.0f, 0.0f, (float)cos(i) * 25.0f), tempRotation = box.GetEuler();
		tempRotation.x += 1;
		tempRotation.y += 1;
		tempRotation.z += 1;
		box.SetPosition(tempPosition, true);
		//box.SetEuler(tempRotation, true);
		box.BuildWorld();


		// [rdr_note] draw Rdr objects

		box.Draw(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
		i += 0.03f;
		sleep(30);
	}
	glfwTerminate();

	return 0;
}
