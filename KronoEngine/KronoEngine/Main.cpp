
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Config.h>

int main(void) {
	GLFWwindow* mWindow;

	if (!glfwInit()) {
		return -1;
	}

	mWindow = glfwCreateWindow(Config::width, Config::height, "KronoEngine", NULL, NULL);

	if (!mWindow) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mWindow);

	GLenum error = glewInit();

	if (error != GLEW_OK) {
		glfwTerminate();
		return -1;
	}

	while (!glfwWindowShouldClose(mWindow)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mWindow);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}