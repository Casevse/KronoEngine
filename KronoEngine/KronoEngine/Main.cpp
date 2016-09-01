
#include <GLFW\glfw3.h>

int main(void) {
	GLFWwindow* mWindow;

	if (!glfwInit()) {
		return -1;
	}

	mWindow = glfwCreateWindow(800, 600, "KronoEngine", NULL, NULL);

	if (!mWindow) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(mWindow);

	while (!glfwWindowShouldClose(mWindow)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mWindow);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}