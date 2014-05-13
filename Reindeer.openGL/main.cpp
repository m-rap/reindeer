#include <GL/gl.h>
#include <GL/glfw3.h>

#include "BoxObject.h"

void initOpenGL() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//float lightAmbient[] = { 0.2f, 0.3f, 0.6f, 1.0f };
	float lightDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float lightPosition[] = { 2.0f, 2.0f, 3.0f, 0.0f };
	float lightDirection[] = { -2.0f, -2.0f, -3.0f };
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);

	float matAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float matDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);

	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
}

int main() {
	if (!glfwInit())
		return 0;

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Our OpenGL Program", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	initOpenGL();

	BoxObject box;
	glm::vec3 position(0.0f, 45.0f, 0.0f), rotation(20, 0, 0);
	box.SetPosition(position);

	Camera camera;
	position.y = 60.0f;
	position.z = -60.0f;
	camera.SetPosition(position);
	camera.SetEuler(rotation);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		box.Draw(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}
