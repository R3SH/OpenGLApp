#include <iostream>
#include <GLFW/glfw3.h>

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit()) return -2;

	window = glfwCreateWindow(640, 480, "Hello world!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2d(-0.9f, -0.5f);
		glVertex2d(0.0f, 0.9f);
		glVertex2d(0.5f, -0.5f);
		glEnd();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}