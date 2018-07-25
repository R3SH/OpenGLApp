#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Windows.h>
#include "renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct shaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static shaderProgramSource parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class shaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	shaderType type = shaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader ") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = shaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = shaderType::FRAGMENT;
		}

		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return{ ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);		// 'i' in "glGetShaderiv()" specifies that it an integer, 'v' - vector;

	if (result == GL_FALSE)		// == !result;
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));		// TODO: search about "alloca" func;
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? " vertex" : " fragment") << " shader!\n";
		std::cout << message << std::endl;

		glDeleteShader(id);
		return 0;
	}

	return id;
}

static int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -2;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	window = glfwCreateWindow(640, 480, "Hello world!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glewInit();

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR\n";

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[8]
		{
			-0.5f, -0.5f,			// 0
				0.5f, -0.5f,		// 1
				0.5f, 0.5f,			// 2
				-0.5f, 0.5f			// 3
		};

		unsigned int indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};

		VertexArray va;
		VertexBuffer vb(positions, 8 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		shaderProgramSource source = parseShader("res/shader/Basic.shader");
		unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shader);

		int location = glGetUniformLocation(shader, "u_Color");
		ASSERT(location != -1);
		glUniform4f(location, 0.2f, 0.5f, 0.1f, 1.0f);

		va.Unbind();
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		float r = 0.0f;
		float inc = 0.01f;

		while (!glfwWindowShouldClose(window))
		{
			if (GetAsyncKeyState((unsigned short)VK_ESCAPE) & 0x8000)
			{
				std::cout << "Ok, bye!" << std::endl;
				return 1;
			}

			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shader);
			glUniform4f(location, r, r * 0.5f, 0.5f, 1.0f);

			va.Bind();
			ib.Bind();

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			if (r > 1.0f)
				inc = -0.01f;
			else if (r < 0.0f)
				inc = 0.01f;

			r += inc;

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		glDeleteProgram(shader);
	}

	glfwTerminate();

	return 0;
}