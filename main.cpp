#include <iostream>
#include <GL\glew.h>
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

	glewInit();

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR\n";

	std::cout << glGetString(GL_VERSION) << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2d(0.2f, -0.2f);
		glVertex2d(0.3f, 0.3f);
		glVertex2d(0.4f, -0.2f);
		glEnd();

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}