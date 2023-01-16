#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <Windows.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestPlainTexture.h"
#include "tests/BatchRenderTest.h"
#include "tests/BatchColorTest.h"
#include "tests/BatchTextureTest.h"
#include "tests/DynamicBatchRenderTest.h"
#include "tests/CoordinateSystemTest.h"
#include "tests/DynamicBatchIBTest.h"
#include "tests/BatchRendererRenderTest.h"
#include "tests/BatchCubeRenderTest.h"
#include "tests/LightingRenderTest.h"

const unsigned int SCREEN_WIDTH = 960;
const unsigned int SCREEN_HEIGHT = 540;

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -2;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello world!", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glewInit();

	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "ERROR\n";

	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;
		//TODO add commandline argument test name input

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");
		testMenu->RegisterTest<test::TestPlainTexture>("Plain Texture");
		testMenu->RegisterTest<test::BatchRenderTest>("Batch Test");
		testMenu->RegisterTest<test::BatchColorTest>("BatchColor Test");
		testMenu->RegisterTest<test::DynamicBatchRenderTest>("Dynamic Batch Render");
		testMenu->RegisterTest<test::DynamicBatchIBTest>("Dynamic Batch IB");
		testMenu->RegisterTest<test::BatchRendererRenderTest>("BatchRendererRenderTest");
		testMenu->RegisterTest<test::BatchCubeRenderTest>("BatchCubeRenderTest");
		testMenu->RegisterTest<test::LightingRenderTest>("LightingRenderTest");
		//testMenu->RegisterTest<test::CoordinateSystemTest>("CoordinateSystem");


		while (!glfwWindowShouldClose(window))
		{
			if (GetAsyncKeyState((unsigned short)VK_ESCAPE) & 0x8000)
			{
				std::cout << "Terminating" << std::endl;
//#ifdef _DEBUG
//				system("PAUSE");
//#endif
				break;
			}

			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();
			if (currentTest)
			{
				currentTest->GetWindow(window);
				//currentTest->Init();
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("TestsMenu");
				if (currentTest != testMenu && ImGui::Button("<-"))
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}
			
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			//Swap front and back buffers
			glfwSwapBuffers(window);

			//Poll for and process events
			glfwPollEvents();
		}

		if (currentTest == testMenu)
			delete testMenu;
		else
			delete currentTest;
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}