#include <stdio.h>
#include <GL/gl.h>
#include <GL/glfw3.h>

#include "BoxObject.h"

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

	BoxObject box;
	glm::vec3 position(0.0f, 0.0f, 0.0f), rotation(30, 0, 0);
	box.SetPosition(position);

	Camera camera;
	position.y = 30;
	position.z = -30;
	camera.SetPosition(position);
	camera.SetEuler(rotation);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		box.Draw(camera);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}
